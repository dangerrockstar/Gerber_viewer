FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    qtbase5-dev \
    qtbase5-dev-tools \
    qttools5-dev \
    libqt5svg5-dev \
    qttools5-dev-tools \
    qt5-qmake \
    pkg-config \
    libopencv-dev \
    libopencv-contrib-dev \
    libgl1-mesa-dev \
    libx11-dev \
    libxcb1-dev \
    libxcb-xinerama0 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxkbcommon-x11-0 \
    libfontconfig1 \
    libglib2.0-0 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . /workspace
RUN mkdir -p /workspace/build

CMD ["bash", "-lc", "mkdir -p build && cd build && qmake .. && make -j$(nproc) && ./geber_app"]
