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

Docker (development)
--------------------

This repository includes a `Dockerfile` and `docker-compose.yml` to build and run the Qt/OpenCV app inside a container. The setup mounts the project source from the host so you can edit code locally and re-run the build inside the container.

Quick commands (from the project root):

- Build image: `docker compose build`
- Run app: `docker compose up`

macOS GUI notes (XQuartz)
-------------------------

To display the Qt GUI from the container on macOS you need an X server such as XQuartz.

1. Install and start XQuartz.
2. In XQuartz preferences → Security, enable "Allow connections from network clients" and restart XQuartz.
3. Allow local connections: `xhost + 127.0.0.1` (or `xhost +` temporarily).
4. Set the DISPLAY environment so the container can reach your host X server:

	`export DISPLAY=host.docker.internal:0`

5. Build and run:

	`docker compose build`
	`docker compose up`

Helper script
-------------

You can use the provided helper script `scripts/run_with_xquartz.sh` which performs the `xhost` step, sets `DISPLAY`, builds, and runs the compose stack.

Notes
-----
- On macOS mounting `/tmp/.X11-unix` into the container usually does not work — the `host.docker.internal:0` DISPLAY approach is recommended.
- If you prefer an interactive shell instead of auto-running the binary, run `docker compose run --rm app bash`.
- Build artifacts are kept in the named volume `gerber_viewer_build_data` and source is mounted from the host so edits are immediate.
