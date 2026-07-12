#include "plot.h"
#include <fstream>
#include <cctype>
#include <cmath>
#include <algorithm>
#include <limits>

namespace {
    double parseGerberCoordinate(const std::string &token)
    {
        if (token.empty())
            return 0.0;

        bool negative = token[0] == '-';
        std::string valueText = negative ? token.substr(1) : token;
        long long value = 0;
        for (char c : valueText) {
            if (std::isdigit(static_cast<unsigned char>(c))) {
                value = value * 10 + (c - '0');
            } else {
                break;
            }
        }

        return (negative ? -1.0 : 1.0) * static_cast<double>(value) / 100.0;
    }
}

void plot::plot_gerber(const std::string &file_path, cv::Mat &surface)
{
    std::ifstream infile(file_path);
    if (!infile.is_open()) {
        cv::putText(surface,
                    "Cannot read Gerber file",
                    cv::Point(16, surface.rows / 2),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.6,
                    cv::Scalar(255),
                    1,
                    cv::LINE_AA);
        return;
    }

    // Read file into memory so we can scan header info first
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }

    // Detect format (FSLAX) and units (MOMM / MOIN)
    int xInt = 2, xDec = 4; // fallback: 2.4
    int yInt = 2, yDec = 4;
    bool unitsMM = false;
    for (const auto &l : lines) {
        if (l.find("%MOMM") != std::string::npos) unitsMM = true;
        size_t p = l.find("%FSLAX");
        if (p != std::string::npos) {
            // expect pattern like %FSLAX66Y66*%
            size_t px = l.find('X', p);
            size_t py = l.find('Y', p);
            if (px != std::string::npos && py != std::string::npos && py > px+1) {
                // read two chars after X as digits (int and dec)
                if (px+2 < l.size() && std::isdigit(static_cast<unsigned char>(l[px+1])) && std::isdigit(static_cast<unsigned char>(l[px+2]))) {
                    xInt = l[px+1] - '0';
                    xDec = l[px+2] - '0';
                }
                if (py+2 < l.size() && std::isdigit(static_cast<unsigned char>(l[py+1])) && std::isdigit(static_cast<unsigned char>(l[py+2]))) {
                    yInt = l[py+1] - '0';
                    yDec = l[py+2] - '0';
                }
            }
        }
    }

    struct GerberCommand {
        double x = 0.0;
        double y = 0.0;
        int d = 2; // 1=draw,2=move,3=flash
    };

    std::vector<GerberCommand> commands;
    double currentX = 0.0;
    double currentY = 0.0;
    int currentD = 2;
    auto parseToken = [&](const std::string &tok, int intDigits, int decDigits) {
        // tok contains only optional sign and digits
        if (tok.empty()) return 0.0;
        bool negative = tok[0] == '-';
        std::string digits;
        for (char c : tok) if (std::isdigit(static_cast<unsigned char>(c))) digits.push_back(c);
        int total = intDigits + decDigits;
        if ((int)digits.size() < total) {
            // pad left with zeros (leading zeros omitted)
            std::string pad(total - digits.size(), '0');
            digits = pad + digits;
        }
        std::string intPart = digits.substr(0, intDigits);
        std::string decPart = digits.substr(intDigits);
        long long iPart = 0;
        long long dPart = 0;
        if (!intPart.empty()) iPart = std::stoll(intPart);
        if (!decPart.empty()) dPart = std::stoll(decPart);
        double value = static_cast<double>(iPart) + static_cast<double>(dPart) / std::pow(10.0, decDigits);
        return negative ? -value : value;
    };

    for (const auto &raw : lines) {
        std::string l = raw;
        // trim
        while (!l.empty() && (l.back() == '\r' || l.back() == '\n' || l.back() == ' ' || l.back() == '\t')) l.pop_back();
        if (l.empty()) continue;
        if (l.front() == '%') continue;
        if (l.front() == 'G' || l.front() == 'M') continue;

        // remove trailing '*'
        if (!l.empty() && l.back() == '*') l.pop_back();

        // pure Dnn aperture selection (no coords)
        if (l.size() > 1 && l[0] == 'D' && std::isdigit(static_cast<unsigned char>(l[1]))) {
            // if it's only Dnn, set currentD or treat as aperture select
            // D01/D02/D03 are stateful commands when used with coords; standalone aperture selects (like D10) we ignore
            if (l.size() >= 3 && (l[1] == '0' || l[1] == '1' || l[1] == '2' || l[1] == '3')) {
                // e.g., D01, D02, D03
                int code = std::stoi(l.substr(1));
                if (code >= 1 && code <= 3) currentD = code;
            }
            continue;
        }

        size_t xPos = l.find('X');
        size_t yPos = l.find('Y');
        size_t dPos = l.find('D');

        // detect inline D code
        if (dPos != std::string::npos && dPos + 1 < l.size()) {
            // read digits after D
            size_t dstart = dPos + 1;
            size_t dend = dstart;
            while (dend < l.size() && std::isdigit(static_cast<unsigned char>(l[dend]))) ++dend;
            if (dend > dstart) {
                int code = std::stoi(l.substr(dstart, dend - dstart));
                currentD = code;
            }
        }

        bool hasX = xPos != std::string::npos;
        bool hasY = yPos != std::string::npos;
        if (hasX) {
            size_t end = (yPos != std::string::npos) ? yPos : (dPos != std::string::npos ? dPos : l.size());
            std::string tok = l.substr(xPos + 1, end - (xPos + 1));
            currentX = parseToken(tok, xInt, xDec);
        }
        if (hasY) {
            size_t end = (dPos != std::string::npos) ? dPos : l.size();
            std::string tok = l.substr(yPos + 1, end - (yPos + 1));
            currentY = parseToken(tok, yInt, yDec);
        }

        if (hasX || hasY) {
            commands.push_back({currentX, currentY, currentD});
        }
    }

    if (commands.empty()) {
        cv::putText(surface,
                    "Gerber file contains no coordinates",
                    cv::Point(16, surface.rows / 2),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.6,
                    cv::Scalar(255),
                    1,
                    cv::LINE_AA);
        return;
    }

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();
    for (const auto &command : commands) {
        minX = std::min(minX, command.x);
        maxX = std::max(maxX, command.x);
        minY = std::min(minY, command.y);
        maxY = std::max(maxY, command.y);
    }
    if (minX == maxX) {
        maxX += 1.0;
    }
    if (minY == maxY) {
        maxY += 1.0;
    }

    const double margin = 20.0;
    double scaleX = (surface.cols - margin * 2) / (maxX - minX);
    double scaleY = (surface.rows - margin * 2) / (maxY - minY);
    double scale = std::min(scaleX, scaleY);
    if (scale <= 0.0) {
        scale = 1.0;
    }

    auto toPoint = [&](double x, double y) {
        int px = static_cast<int>((x - minX) * scale + margin + 0.5);
        int py = static_cast<int>(surface.rows - ((y - minY) * scale + margin) - 0.5);
        return cv::Point(px, py);
    };

    // Interpret D-codes across the sequence: D02=move, D01=draw, D03=flash
    double lastX = commands.front().x;
    double lastY = commands.front().y;
    int lastD = commands.front().d;
    for (size_t i = 0; i < commands.size(); ++i) {
        const auto &cmd = commands[i];
        cv::Point pt = toPoint(cmd.x, cmd.y);
        if (cmd.d == 1) {
            // draw from lastX,lastY to current
            cv::Point lastPt = toPoint(lastX, lastY);
            cv::line(surface, lastPt, pt, cv::Scalar(255), 1, cv::LINE_AA);
            lastX = cmd.x;
            lastY = cmd.y;
            lastD = cmd.d;
        } else if (cmd.d == 2) {
            // move
            lastX = cmd.x;
            lastY = cmd.y;
            lastD = cmd.d;
        } else if (cmd.d == 3) {
            // flash - draw a filled circle (approx aperture)
            cv::circle(surface, pt, 3, cv::Scalar(255), cv::FILLED, cv::LINE_AA);
            lastX = cmd.x;
            lastY = cmd.y;
            lastD = cmd.d;
        }
    }
}


