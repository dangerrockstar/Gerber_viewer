# Gerber Viewer (RS-274X)

A lightweight, high-performance Printed Circuit Board (PCB) Gerber RS-274X file viewer built using C++, Qt 5, and OpenCV. 

Unlike traditional vector graphic frameworks, this viewer leverages OpenCV's highly efficient image processing matrices (cv::Mat) to rasterize and render Gerber geometric commands, offering smooth manipulation and custom handling of complex board elements.

---

## Features

- Gerber RS-274X Parsing: Reads standard Gerber commands, extracting board parameters, trace geometry, and polarity options.
- OpenCV Accelerated Rendering: Utilizes standard open-source vision libraries to cleanly rasterize paths, lines, and complex polygons into memory surfaces.
- High-Fidelity Arc Calculation: Features custom internal interpolation geometry algorithms for precise clockwise and counter-clockwise circular interpolation (G02/G03 equivalents).
- Interactive Qt 5 Interface: Includes a responsive workspace featuring a specialized viewport supporting dynamic scaling, smooth mouse-panning, and zoom-to-cursor utilities.

---

## Architecture Overview

The codebase separates the frontend presentation layer, file parser, and graphic rendering engine into clear modular sections:

├── main.cpp                  # Application lifecycle entry point
├── mainwindow.cpp/.h/.ui     # Main UI window, orchestrates canvas sizing & workflow
├── qsmartgraphicsview.cpp/.h # Custom Qt Graphic View widget handling zoom/pan events
├── file_reading.cpp/.h       # Low-level Gerber text parser calculating spatial limits
├── plot.cpp/.h               # Rasterizes Gerber aperture and flash commands to surface
└── dataset_plot.cpp/.h       # Mathematical calculation layer for arc vertices and contour data

### Component Breakdown

1. file_reading::face(...)
   Pre-scans the target text file to automatically compute boundaries (xmin, xmax, ymin, ymax) and establish correct copper layer polarities (dark vs. clear).
2. plot::plot_gerber(...)
   Translates individual flash and line drawing instructions directly onto a cv::Mat graphic array buffer.
3. dataset_plot::arc_refined_new(...)
   Interpolates circular arcs by mapping start/end coordinates and center offsets (I/J parameters) into dense point matrices for OpenCV outline rendering.

---

## Running with Docker & Automation Scripts

The project includes an integrated setup to build and run the application inside a container using pre-configured automated scripts, ensuring an identical run-environment regardless of your host setup.

### 1. Build and Run via Automated Scripts
Execute the embedded build and run scripts to automate dependency mapping and setup:

# Render execution permissions if necessary
chmod +x build_docker.sh run_image.sh

# Run the build script to compile the workspace into the image container
./build_docker.sh

# Execute the run script to start the container and launch the interface
./run_image.sh


### 2. Manual Commands & GUI Display Forwarding
If you prefer running the container commands manually, ensure display forwarding is enabled to let the container render the Qt GUI window onto your host screen.

#### Linux (X11)
# Allow local container connections to the X server
xhost +local:docker

# Run the container mapping the display environment and socket
docker run -it --rm \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    gerber-viewer

# Revoke permissions after closing the application
xhost -local:docker

#### macOS (Using XQuartz)
1. Install and launch XQuartz.
2. Go to XQuartz Preferences > Security, and check "Allow connections from clients".
3. Run the following in your terminal:

# Allow connections from your local IP address
xhost + 127.0.0.1

# Run the container targeting your host's network display address
docker run -it --rm \
    -e DISPLAY=host.docker.internal:0 \
    gerber-viewer

#### Windows (WSL2 / VcXsrv)
- For WSL2 with Windows 11, GUI rendering is supported natively. Simply run the container via your WSL terminal using Linux instructions.
- For older environments, launch an X server (like VcXsrv or Xming) with "Disable access control" checked, and set your display variable:
docker run -it --rm -e DISPLAY=host.docker.internal:0 gerber-viewer

---

## Multi-OS Cross-Platform Local Installation

If you prefer compiling the application natively without Docker, follow the instructions tailored for your specific operating system:

### Dependencies
- Compiler: C++11 compliant compiler (GCC, Clang, or MSVC)
- Framework: Qt 5.x (including Qt Creator IDE if preferred)
- Libraries: OpenCV 3.x or 4.x (Core, Imgproc, and Highgui modules)

### OS-Specific Installation Guides

### Linux (Ubuntu/Debian)
1. Install system toolchains and developer libraries:
   sudo apt-get update
   sudo apt-get install -y build-essential qtbase5-dev qt5-qmake libopencv-dev libgl1-mesa-glx
2. Build the project:
   mkdir build && cd build
   qmake ../geber_app.pro
   make
   ./geber_app

### macOS (Intel / Apple Silicon)
1. Install dependencies via Homebrew:
   brew install qt@5 opencv
2. Link Qt5 to your system path (if Homebrew hasn't already done so):
   export PATH="/opt/homebrew/opt/qt@5/bin:$PATH"
3. Update the geber_app.pro configuration file to locate Homebrew dependencies:
   macx {
       INCLUDEPATH += /opt/homebrew/include/opencv4
       LIBS += -L/opt/homebrew/lib -lopencv_core -lopencv_imgproc -lopencv_highgui
   }
4. Build the application:
   mkdir build && cd build
   qmake ../geber_app.pro
   make
   open geber_app.app

### Windows (MSVC / MinGW)
1. Download and run the Qt Online Installer to set up Qt 5.x.
2. Download and extract pre-compiled OpenCV binaries from the official site, or build via vcpkg:
   vcpkg install opencv:x64-windows qt5-base:x64-windows
3. Open Qt Creator, load the geber_app.pro project file.
4. Update the .pro path settings to point to your local OpenCV Windows directory:
   win32 {
       INCLUDEPATH += C:/opencv/build/include
       LIBS += -LC:/opencv/build/x64/vc15/lib -lopencv_worldXX
   }
5. Choose your build environment (MinGW or MSVC Kit) and press Ctrl + R to compile and launch.

---

## License

This project is open-source. Please review the repository details or contact the author for specific licensing parameters.