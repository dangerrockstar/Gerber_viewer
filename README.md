# Gerber_viewer
Gerber274x viewer


Gerber 247X With Opencv

Project Recruitment Tools-
1) Opencv
2) C++
3) Qt 5x






File information 

1) file_reading.cpp for surface only.
2) mainwindow.cpp 
3) plot.cpp file for Ploting object into Surface.
4) dataset_plot.cpp for draw Arc function.
5) main.cpp


1)  file_reading.cpp-
void file_reading::face(string file_path , float &xmax ,float &ymax, float &ymin , float &xmin , bool &polarity )

Parameters- 

A) string file_path -   path of directory.
B) float &xmax - call by reference float value return  x axis max.
C) float &ymax - call by reference float value return  y axis max.
D) float &ymin - call by reference float value return  y axis min.
E) float &xmin - call by reference float value return  x axis min.
F) bool &polarity – call by reference bool value return PCB polarity black or white .



3) plot.cpp- void plot:: plot_gerber(std::string file_path , cv::Mat&surface )

Parameters- 
A) std::string file_path - Path of directory
B) cv::Mat&surface – Pass surface .


2) mainwindow.cpp-  Here we create surface using weight- hight ( max mix of axis ) after that pass this surface into plot.cpp file and some GUI part.


4) dataset_plot.cpp - void dataset_plot::arc_refined_new(bool cw,double x1, double y1, double x2, double y2, double c1, double c2,vector<cv::Point> &vertices)

A) bool cw – G-code from gerber 247X clockwise and anti clockwise.
B) double x1 – vector value start point X
C) double y1 - vector value start point Y
D) double x2 - vector value End point X
E) double y2 - vector value End point Y
F) double c1 - vector value I
G) double c2 - vector value J
H) vector<cv::Point> &vertices – Return all  vertices's for drawContour   opencv function 



5) main.cpp- a main function of Application.
