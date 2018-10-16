FROM ubuntu:16.04
LABEL maintainer="kewagi"
LABEL version="2.0.0.0"
LABEL name="Bulwark"

ENV BULWARK_VERSION 2.0.0.0
ENV CXXFLAGS "-Wall -Wextra -Wformat -Wformat-security -Wno-unused-parameter -Wno-narrowing"

RUN apt-get update \
  && apt-get -y install --no-install-recommends \
  software-properties-common=0.96.20.7 \
  && add-apt-repository -y ppa:bitcoin/bitcoin \
  && apt-get update \
  && apt-get -y install --no-install-recommends \
  autoconf=2.69-9 \
  automake=1:1.15-4ubuntu1 \
  autotools-dev=20150820.1 \
  bsdmainutils=9.0.6ubuntu3 \
  build-essential=12.1ubuntu2 \
  curl=7.47.0-1ubuntu2.9 \
  libboost-all-dev=1.58.0.1ubuntu1 \
  libdb4.8-dev=4.8.30-xenial4 \ 
  libdb4.8++-dev=4.8.30-xenial4 \
  libevent-dev=2.0.21-stable-2ubuntu0.16.04.1 \
  libminiupnpc-dev=1.9.20140610-2ubuntu2.16.04.2 \
  libprotobuf-dev=2.6.1-1.3 \
  libqrencode-dev=3.4.4-1 \
  libqt5core5a=5.5.1+dfsg-16ubuntu7.5 \
  libqt5dbus5=5.5.1+dfsg-16ubuntu7.5 \
  libqt5gui5=5.5.1+dfsg-16ubuntu7.5 \
  libssl-dev=1.0.2g-1ubuntu4.13 \
  libtool=2.4.6-0.1 \
  libzmq5=4.1.4-7 \
  libzmq-dev=2.2.0+dfsg-7 \
  pkg-config=0.29.1-0ubuntu1 \
  protobuf-compiler=2.6.1-1.3 \
  qttools5-dev-tools=5.5.1-3ubuntu0.1 \
  qttools5-dev=5.5.1-3ubuntu0.1 \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* \
  && mkdir /release

COPY . /Bulwark
WORKDIR /Bulwark

RUN ./autogen.sh \
  && ./configure \
  --enable-zmq \
  --enable-glibc-back-compat \
  --enable-reduce-exports \
  --disable-shared \
  --enable-hardening \
  --with-miniupnpc \
  --enable-upnp-default \
  && make 

WORKDIR /release

RUN  cp /Bulwark/src/bulwark-cli /Bulwark/src/bulwarkd /Bulwark/src/bulwark-tx /Bulwark/src/qt/bulwark-qt . \
  && strip ./* \
  && tar czf bulwark.tar.gz ./* \
  && rm bulwark-cli bulwark-qt bulwark-tx bulwarkd

ENTRYPOINT ["/bin/sh"]
