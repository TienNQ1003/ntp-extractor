FROM ubuntu:18.04 

WORKDIR /home/video_cap

COPY . /home/video_cap

# Install dependencies
RUN mkdir -p /home/video_cap && \
  cd /home/video_cap && \
  chmod +x install.sh && \
  ./install.sh

# Install debugging tools
RUN apt-get update && \
  apt-get -y install \
  gdb \
  python3-dbg

# install Python
RUN apt-get update && \
  apt-get upgrade -y && \
  apt-get install -y \
    pkg-config \
    python3-dev \
    python3-pip \
    python3-numpy \
    python3-pkgconfig && \
    rm -rf /var/lib/apt/lists/*

RUN apt-get update && \
  apt-get -y install \
    libgtk-3-dev \
    libavcodec-dev \
    libavformat-dev \
    libswscale-dev \
    libmp3lame-dev \
    zlib1g-dev \
    libx264-dev \
    libsdl2-dev \
    libvpx-dev \
    libvdpau-dev \
    libvorbis-dev \
    libopus-dev \
    libdc1394-22-dev \
    liblzma-dev && \
    rm -rf /var/lib/apt/lists/*

# Set environment variables
ENV PATH="$PATH:/home/bin"
ENV PKG_CONFIG_PATH="$PKG_CONFIG_PATH:/home/ffmpeg_build/lib/pkgconfig"
ENV LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/opencv/build/lib"

# Install Python package
RUN cd /home/video_cap && \
  python3 setup.py install

RUN pip3 install -r requirements.txt

CMD ["sh", "-c", "tail -f /dev/null"]
