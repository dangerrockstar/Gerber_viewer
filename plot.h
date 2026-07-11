#ifndef PLOT_H
#define PLOT_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class plot
{
public:
    void plot_gerber(const std::string &file_path, cv::Mat &surface);
};

#endif // PLOT_H
