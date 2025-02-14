# Use an official Ubuntu base image (20.04 or later)
FROM ubuntu:20.04

# Disable interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update packages and install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    libblas-dev \
    libboost-filesystem-dev \
    libboost-system-dev \
    libboost-thread-dev \
    libglu1-mesa-dev \
    libsuitesparse-dev \
    xorg-dev \
    ccache \
    libeigen3-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory for the build
WORKDIR /app

# Clone the fTetWild repository (including submodules)
RUN git clone https://github.com/wildmeshing/fTetWild.git --recursive

# Create a build directory and run CMake configuration and build.
WORKDIR /app/fTetWild/build
# Disable Python bindings by setting FLOAT_TETWILD_ENABLE_PYBINDINGS=OFF
RUN cmake .. -DFLOAT_TETWILD_ENABLE_PYBINDINGS=OFF && make -j$(nproc)

# Set the entrypoint to the fTetWild executable.
ENTRYPOINT ["/app/fTetWild/build/FloatTetwild_bin"]
