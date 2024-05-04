# Cross Compilation

Since this project uses some dependencies which take a long time to compile on a lower power device like a Raspberry Pi Zero, it is valuable to setup a cross-compilation environment. This document collects some details and hints to setup the necessary. These steps have been used on a Windows machine using WSL2 with a Debian Bookworm 12.5.

```sh
sudo apt update 
sudo apt install build-essential cmake ubuntu-dev-tools
```

```sh
export ARCH=arm64
export RELEASE=bullseye
export TC=aarch64-rpi3-linux-gnu
export PATH="$HOME/opt/x-tools/$TC/bin:$PATH"
cmake -S. -Bbuild -DCMAKE_TOOLCHAIN_FILE="../cmake/aarch64-rpi2zero-linux-gnu.cmake"
scp /home/musicbox/opt/x-tools/aarch64-rpi3-linux-gnu/aarch64-rpi3-linux-gnu/sysroot/lib/libstdc++.so.6.0.32 music@192.168.135.51:~

sudo mkdir -p ~/chroots/rpi-bullseye-arm64/usr/local/lib/aarch64-linux-gnu
sudo cp ~/opt/x-tools/aarch64-rpi3-linux-gnu/aarch64-rpi3-linux-gnu/sysroot/lib/libstdc++.so.6.0.32 ~/chroots/rpi-bullseye-arm64/usr/local/lib/aarch64-linux-gnu
sudo schroot -c source:rpi-$RELEASE-$ARCH -u root -d / ldconfig

sudo sbuild-apt rpi-bullseye-arm64 apt-get install libboost-all-dev
```

deploy the same libstdc++ on the Raspberry Pi

```sh
sudo mkdir -p /usr/local/lib/aarch64-linux-gnu/
sudo mv libstdc++.so.6.0.32 /usr/local/lib/aarch64-linux-gnu/
sudo ldconfig
```


## bcm2835 

```sh
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.73.tar.gz
tar zxvf bcm2835-1.73.tar.gz
cd bcm2835-1.73/
export ARCH=arm64
export RELEASE=bullseye
export TC=aarch64-rpi3-linux-gnu
export PATH="$HOME/opt/x-tools/$TC/bin:$PATH"
./configure -host=arm -prefix=$(dirname ~/chroots/rpi-bullseye-arm64/usr) CC=aarch64-rpi3-linux-gnu-gcc ar=aarch64-rpi3-linux-gnu-ar CFLAGS="-DBCM2835_HAVE_LIBCAP --sysroot=/home/musicbox/chroots/rpi-bullseye-arm64" LDFLAGS="-lcap"
make
sudo make install
```


## Reference Documentation
* https://github.com/tttapa/docker-arm-cross-toolchain
* https://tttapa.github.io/Pages/Raspberry-Pi/C++-Development-RPiOS/Building-example-project.html
* Fix lpthread link (and others in chroot env): https://github.com/tttapa/RPi-Cross-Cpp-Development/issues/2#issuecomment-1431703892