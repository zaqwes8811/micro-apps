FROM ubuntu:24.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

# Install g++-14 compiler and the C++14/17/20 standard library
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    g++-14 \
    libstdc++-14-dev \
    && rm -rf /var/lib/apt/lists/*

# Verify installation with coroutine support
RUN g++-14 --version && \
    g++-14 -fcoroutines -std=c++20 -fsyntax-only /dev/null 2>&1 | head -5

# Set default shell
SHELL ["/bin/bash", "-c"]

# Set default shell
SHELL ["/bin/bash", "-c"]
