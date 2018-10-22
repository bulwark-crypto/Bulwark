FROM ubuntu:16.04 AS base
RUN dpkg --add-architecture i386 \
  && apt-get update \
  && apt-get -y install --no-install-recommends \
  software-properties-common=0.96.20.7 \
  && add-apt-repository -y ppa:bitcoin/bitcoin \
  && apt-get update \
  && apt-get -y install --no-install-recommends \
  autoconf=2.69-9 \
  automake=1:1.15-4ubuntu1 \
  autotools-dev=20150820.1 \
  binutils-mingw-w64-i686=2.26-3ubuntu1+6.6 \
  binutils-mingw-w64-x86-64=2.26-3ubuntu1+6.6 \
  bsdmainutils=9.0.6ubuntu3 \
  build-essential=12.1ubuntu2 \
  curl=7.47.0-1ubuntu2.9 \
  g++-5-multilib-arm-linux-gnueabihf=5.4.0-6ubuntu1~16.04.9cross1 \
  g++-5-multilib=5.4.0-6ubuntu1~16.04.10 \
  g++-arm-linux-gnueabihf=4:5.3.1-1ubuntu1 \
  g++-mingw-w64-i686=5.3.1-8ubuntu3+17 \
  g++-mingw-w64-x86-64=5.3.1-8ubuntu3+17 \
  gcc-5-multilib-arm-linux-gnueabihf=5.4.0-6ubuntu1~16.04.9cross1 \
  gcc-5-multilib=5.4.0-6ubuntu1~16.04.10 \
  libboost-all-dev=1.58.0.1ubuntu1 \
  libc6-dev:i386=2.23-0ubuntu10 \
  libdb4.8-dev=4.8.30-xenial4 \ 
  libdb4.8++-dev=4.8.30-xenial4 \
  libevent-dev=2.0.21-stable-2ubuntu0.16.04.1 \
  libjpeg-dev=8c-2ubuntu8 \
  libminiupnpc-dev=1.9.20140610-2ubuntu2.16.04.2 \
  libprotobuf-dev=2.6.1-1.3 \
  libqrencode-dev=3.4.4-1 \
  libqt5core5a=5.5.1+dfsg-16ubuntu7.5 \
  libqt5dbus5=5.5.1+dfsg-16ubuntu7.5 \
  libqt5gui5=5.5.1+dfsg-16ubuntu7.5 \
  libssl-dev=1.0.2g-1ubuntu4.13 \
  libtool=2.4.6-0.1 \
  libz-mingw-w64-dev=1.2.8+dfsg-2 \
  libzmq-dev=2.2.0+dfsg-7 \
  libzmq5=4.1.4-7 \
  linux-libc-dev:i386=4.4.0-137.163 \
  linux-libc-dev=4.4.0-137.163 \
  mingw-w64-i686-dev=4.0.4-2 \
  mingw-w64-x86-64-dev=4.0.4-2 \
  openssl=1.0.2g-1ubuntu4.13 \
  pkg-config=0.29.1-0ubuntu1 \
  protobuf-compiler=2.6.1-1.3 \
  qttools5-dev-tools=5.5.1-3ubuntu0.1 \
  qttools5-dev=5.5.1-3ubuntu0.1 \
  unzip=6.0-20ubuntu1 \
  zlib1g-dev=1:1.2.8.dfsg-2ubuntu4.1 \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/* \
  && mkdir /Bulwark \
  && update-alternatives --set x86_64-w64-mingw32-g++ /usr/bin/x86_64-w64-mingw32-g++-posix \
  && update-alternatives --set i686-w64-mingw32-g++ /usr/bin/i686-w64-mingw32-g++-posix
COPY depends /Bulwark/depends
RUN make -C /Bulwark/depends HOST=i686-w64-mingw32 \
  && make -C /Bulwark/depends HOST=i686-pc-linux-gnu \
  && make -C /Bulwark/depends HOST=x86_64-unknown-linux-gnu \
  && make -C /Bulwark/depends HOST=x86_64-w64-mingw32 \
  && make -C /Bulwark/depends HOST=arm-linux-gnueabihf

FROM base AS scaffold
COPY autogen.sh /Bulwark/
COPY build-aux /Bulwark//build-aux
COPY configure.ac /Bulwark/
COPY contrib /Bulwark//contrib
COPY Makefile.am /Bulwark/
COPY pkg.m4 /Bulwark/
COPY qa /Bulwark/qa
COPY share /Bulwark//share
COPY src /Bulwark/src
WORKDIR /Bulwark

FROM scaffold AS arm32
ENV CXXFLAGS "-Wall -Wextra -Wformat -Wformat-security -Wno-unused-parameter -Wno-narrowing"
RUN ./autogen.sh \
  && ./configure \
  --enable-zmq \
  --enable-glibc-back-compat \
  --enable-reduce-exports \
  --disable-shared \
  --enable-hardening \
  --disable-tests \
  --with-miniupnpc \
  --enable-upnp-default \
  --prefix="$(pwd)/depends/arm-linux-gnueabihf" \
  && make \
  && cp ./src/bulwark-cli ./src/bulwarkd ./src/qt/bulwark-qt / \
  && make clean
WORKDIR /
RUN arm-linux-gnueabihf-strip ./bulwark-cli ./bulwark-qt ./bulwarkd  \
  && tar czf arm32.tar.gz ./bulwark* \
  && rm ./bulwark-cli ./bulwark-qt ./bulwarkd 

FROM scaffold AS linux32
ENV CXXFLAGS "-Wall -Wextra -Wformat -Wformat-security -Wno-unused-parameter -Wno-narrowing"
RUN ./autogen.sh \
  && ./configure \
  --enable-zmq \
  --with-pic \
  --enable-glibc-back-compat \
  --enable-reduce-exports \
  --disable-shared \
  --enable-hardening \
  --disable-tests \
  --with-miniupnpc \
  --enable-upnp-default \
  --prefix="$(pwd)/depends/i686-pc-linux-gnu" \
  && make \
  && cp ./src/bulwark-cli ./src/bulwarkd ./src/qt/bulwark-qt / \
  && make clean
WORKDIR /
RUN strip ./bulwark-cli ./bulwark-qt ./bulwarkd  \
  && tar czf linux32.tar.gz ./bulwark* \
  && rm ./bulwark-cli ./bulwark-qt ./bulwarkd 

FROM scaffold AS linux64
ENV CXXFLAGS "-Wall -Wextra -Wformat -Wformat-security -Wno-unused-parameter -Wno-narrowing"
RUN ./autogen.sh \
  && ./configure \
  --enable-zmq \
  --with-pic \
  --enable-glibc-back-compat \
  --enable-reduce-exports \
  --disable-shared \
  --enable-hardening \
  --disable-tests \
  --with-miniupnpc \
  --enable-upnp-default \
  --prefix="$(pwd)/depends/x86_64-unknown-linux-gnu" \
  && make \
  && cp ./src/bulwark-cli ./src/bulwarkd ./src/qt/bulwark-qt / \
  && make clean
WORKDIR /
RUN strip ./bulwark-cli ./bulwark-qt ./bulwarkd  \
  && tar czf linux64.tar.gz ./bulwark* \
  && rm ./bulwark-cli ./bulwark-qt ./bulwarkd 

FROM scaffold AS windows32
ENV CXXFLAGS "-Wall -Wextra -Wformat -Wformat-security -Wno-unused-parameter -Wno-narrowing"
COPY autogen.sh .
COPY build-aux ./build-aux
COPY configure.ac .
COPY contrib ./contrib
COPY Makefile.am .
COPY pkg.m4 .
COPY qa ./qa
COPY share ./share
COPY src ./src
RUN ./autogen.sh \
  && ./configure \
  --enable-gui \
  --enable-reduce-exports \
  --disable-shared \
  --with-pic \
  --disable-tests \
  --enable-upnp-default \
  --prefix="$(pwd)/depends/i686-w64-mingw32" \
  && make \
  && cp ./src/bulwark-cli.exe ./src/bulwarkd.exe ./src/qt/bulwark-qt.exe / \
  && make clean
WORKDIR /
RUN strip ./bulwark-cli.exe ./bulwark-qt.exe ./bulwarkd.exe  \
  && tar czf windows32.tar.gz ./bulwark* \
  && rm ./bulwark-cli.exe ./bulwark-qt.exe ./bulwarkd.exe

FROM scaffold AS windows64
ENV CXXFLAGS "-Wall -Wextra -Wformat -Wformat-security -Wno-unused-parameter -Wno-narrowing"
COPY autogen.sh .
COPY build-aux ./build-aux
COPY configure.ac .
COPY contrib ./contrib
COPY Makefile.am .
COPY pkg.m4 .
COPY qa ./qa
COPY share ./share
COPY src ./src
RUN ./autogen.sh \
  && ./configure \
  --enable-gui \
  --enable-reduce-exports \
  --disable-shared \
  --with-pic \
  --disable-tests \
  --enable-upnp-default \
  --prefix="$(pwd)/depends/x86_64-w64-mingw32" \
  && make \
  && cp ./src/bulwark-cli.exe ./src/bulwarkd.exe ./src/qt/bulwark-qt.exe / \
  && make clean
WORKDIR /
RUN strip ./bulwark-cli.exe ./bulwark-qt.exe ./bulwarkd.exe  \
  && tar czf windows64.tar.gz ./bulwark* \
  && rm ./bulwark-cli.exe ./bulwark-qt.exe ./bulwarkd.exe

FROM alpine:3.8
LABEL maintainer="kewagi"
LABEL version="2.0.0.0"
LABEL name="Bulwark Docker Builds"
RUN mkdir /release
COPY --from=arm32 /arm32.* /release
COPY --from=linux32 /linux32.* /release
COPY --from=linux64 /linux64.* /release
COPY --from=windows32 /windows32.* /release
COPY --from=windows64 /windows64.* /release

ENTRYPOINT ["ash"]