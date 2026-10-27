FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install compiler, build tools, git, SSL certificates, and debugging utilities
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        g++ \
        make \
        cmake \
        git \
        ca-certificates \
        valgrind \
        gdb && \
    rm -rf /var/lib/apt/lists/*

# Build and install GoogleTest v1.12.1 configured explicitly for C++11
# Build and install GoogleTest release-1.12.1 configured explicitly for C++11
RUN git clone --depth=1 --branch release-1.12.1 https://github.com/google/googletest.git /tmp/googletest && \
    cd /tmp/googletest && \
    cmake -Bbuild -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=11 && \
    cmake --build build --target install && \
    ldconfig && \
    rm -rf /tmp/googletest

WORKDIR /app

# Copy repository structure
COPY Makefile ./
COPY src/ ./src/
COPY testing/ ./testing/

CMD ["make", "test"]