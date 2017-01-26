FROM debian:stretch

RUN apt-get update && apt-get install -y \
    build-essential \
    ca-certificates \
    cmake \
    curl \
    git \
    librocksdb-dev \
    --no-install-recommends && \
    rm -rf /var/lib/apt/lists/*

# Get actor framwork
WORKDIR /scratch
RUN curl -LO https://github.com/actor-framework/actor-framework/archive/0.14.5.tar.gz
RUN tar xzf 0.14.5.tar.gz
WORKDIR actor-framework-0.14.5
RUN ./configure
RUN make -j4 install

# Get broker repository
WORKDIR /scratch
RUN git clone --recursive https://github.com/bro/broker /scratch/broker-git
WORKDIR broker-git

# Use correct branches / submodules
RUN git checkout topic/mfischer/broker-multihop
# tie our version
RUN git submodule update
RUN git checkout 881b4f4
RUN ./configure
RUN make -j4 install

# Remove sources
RUN rm -rf /scratch

RUN mkdir /opt/acu /opt/acu-fw

# well not perfect, but somehow we have to get our lib in there
COPY acu-fw/CMakeLists.txt /opt/acu-fw
COPY acu-fw/Makefile /opt/acu-fw
COPY acu-fw/test /opt/acu-fw/test
COPY acu-fw/include /opt/acu-fw/include
COPY acu-fw/cmake /opt/acu-fw/cmake
COPY acu-fw/src /opt/acu-fw/src

WORKDIR /opt/acu-fw
RUN make

EXPOSE 9999

COPY acu/CMakeLists.txt /opt/acu
COPY acu/Makefile /opt/acu
COPY acu/test /opt/acu/test
COPY acu/cmake /opt/acu/cmake
COPY acu/config-local.ini /opt/acu
COPY acu/src /opt/acu/src

WORKDIR /opt/acu
RUN make

CMD ["build/src/acu-impl", "config-local.ini"]