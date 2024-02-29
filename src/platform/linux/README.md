# GowEngine Linux

### Environment Setup
```bash
sudo apt update -y \
&& sudo apt install -y build-essential libtool autoconf unzip wget xorg-dev libglew-dev libpulse-dev python3
```

### Install Code::Blocks IDE
```bash
sudo add-apt-repository ppa:codeblocks-devs/release \
&& sudo apt update \
&& sudo apt install codeblocks codeblocks-contrib
```

### Install Cmake
Remove default version of cmake if it exists:
```bash
sudo apt remove --purge --auto-remove cmake
```

Download, extract, and build CMake:
```bash
version=3.26.4 \
&& mkdir ~/temp \
&& cd ~/temp \
&& wget https://github.com/Kitware/CMake/releases/download/v$version/cmake-$version.tar.gz \
&& tar -xzvf cmake-$version.tar.gz \
&& cd cmake-$version/ \
&& ./bootstrap -- -DCMAKE_USE_OPENSSL=OFF \
&& make -j$(nproc) \
&& sudo make install \
&& cmake --version
```
