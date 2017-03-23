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
COPY beemaster-acu-fw/CMakeLists.txt /opt/acu/beemaster-acu-fw/
COPY beemaster-acu-fw/Makefile /opt/acu/beemaster-acu-fw/
COPY beemaster-acu-fw/test /opt/acu/beemaster-acu-fw/test
COPY beemaster-acu-fw/include /opt/acu/beemaster-acu-fw/include
COPY beemaster-acu-fw/cmake /opt/acu/beemaster-acu-fw/cmake
COPY beemaster-acu-fw/src /opt/acu/beemaster-acu-fw/src

COPY CMakeLists.txt /opt/acu
COPY Makefile /opt/acu
COPY test /opt/acu/test
COPY cmake /opt/acu/cmake
COPY src /opt/acu/src
COPY config-local.ini /opt/acu

WORKDIR /opt/acu
RUN make

ENTRYPOINT ["build/src/acu-impl"]
CMD ["config-local.ini"]
