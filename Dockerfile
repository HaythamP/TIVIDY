FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive


RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        g++ \
        make \
        cmake \
        git \
        ca-certificates \
        valgrind \
        gdb \
        curl \
        libasio-dev && \
    rm -rf /var/lib/apt/lists/*


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
COPY public/ ./public/
COPY samples/ ./samples/

RUN curl -L -o src/crow.h https://github.com/CrowCpp/Crow/releases/latest/download/crow_all.h

RUN mkdir -p bin build && \
    g++ -std=c++17 -Isrc -Isrc/include \
        $(find src -maxdepth 1 -name "*.cpp" ! -name "main.cpp") \
        -o bin/tividy_server -pthread

EXPOSE 8080

#CMD ["make", "test"]
CMD ["./bin/tividy_server"]