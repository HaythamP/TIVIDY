FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \ 
    apt-get install -y --no-install-recommends \
    g++ \
    make \
    cmake \ 
    libgtest-dev \
    valgrind \
    gdb && \
    rm -rf /var/lib/apt/lists/*

RUN cd /usr/src/googletest && \
    cmake -Bbuild -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --target install && \
    ldconfig


WORKDIR /app

COPY Makefile ./
COPY src/ ./src/


CMD ["make"]