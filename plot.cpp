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

    struct GerberCommand {
        double x = 0.0;
        double y = 0.0;
        int d = 2;
    };

    std::vector<GerberCommand> commands;
    double currentX = 0.0;
    double currentY = 0.0;
    int currentD = 2;
    std::string line;

    while (std::getline(infile, line)) {
        if (line.empty())
            continue;

        if (line.front() == '%' || line.front() == 'G' || line.front() == 'M')
            continue;

        size_t xPos = line.find('X');
        size_t yPos = line.find('Y');
        size_t dPos = line.find('D');

        if (dPos != std::string::npos && dPos + 1 < line.size()) {
            std::string dCode = line.substr(dPos + 1, 2);
            if (dCode.size() == 2 && std::isdigit(static_cast<unsigned char>(dCode[0])) && std::isdigit(static_cast<unsigned char>(dCode[1]))) {
                currentD = std::stoi(dCode);
            }
        }

        bool hasCoordinate = false;
        if (xPos != std::string::npos) {
            size_t end = (yPos != std::string::npos) ? yPos : (dPos != std::string::npos ? dPos : line.size());
            currentX = parseGerberCoordinate(line.substr(xPos + 1, end - (xPos + 1)));
            hasCoordinate = true;
        }
        if (yPos != std::string::npos) {
            size_t end = (dPos != std::string::npos) ? dPos : line.size();
            currentY = parseGerberCoordinate(line.substr(yPos + 1, end - (yPos + 1)));
            hasCoordinate = true;
        }

        if (hasCoordinate) {
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

    cv::Point previousPoint = toPoint(commands[0].x, commands[0].y);
    for (size_t i = 1; i < commands.size(); ++i) {
        const auto &command = commands[i];
        cv::Point currentPoint = toPoint(command.x, command.y);
        if (command.d == 1) {
            cv::line(surface, previousPoint, currentPoint, cv::Scalar(255), 1, cv::LINE_AA);
        } else if (command.d == 3) {
            cv::circle(surface, currentPoint, 3, cv::Scalar(255), cv::FILLED, cv::LINE_AA);
        }
        previousPoint = currentPoint;
    }
}


