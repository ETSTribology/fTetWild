FROM ubuntu
ENV DEBIAN_FRONTEND=noninteractive

RUN sudo apt-get update
RUN sudo apt-get install \
    libblas-dev \
    libboost-filesystem-dev \
    libboost-system-dev \
    libboost-thread-dev \
    libglu1-mesa-dev \
    libsuitesparse-dev \
    xorg-dev \
    ccache

WORKDIR /app

RUN git clone https://github.com/wildmeshing/fTetWild.git --recursive
WORKDIR /app/fTetWild/build
RUN cmake .. && make

WORKDIR /data

ENTRYPOINT ["/app/fTetWild/build/FloatTetwild_bin"]
