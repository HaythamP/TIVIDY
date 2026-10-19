FROM ubuntu:22.04

RUN apt-get update && \ 
    apt-get install -y --no-install-recommends g++ make valgrind gdb && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY Makefile ./
COPY src/ ./src/

RUN make clean && make
CMD ["./TIVIDY"]