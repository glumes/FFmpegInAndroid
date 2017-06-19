#!/bin/bash

NDK=/Users/glumes/Library/Android/sdk/ndk-bundle
SYSROOT=$NDK/platforms/android-14/arch-arm/
TOOLCHAIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64

CPU=arm
PREFIX=$(pwd)/android/$CPU
ADDI_CFLAGS=""
ADDI_LDFLAGS=""

function build_arm
{
./configure \
    --prefix=$PREFIX \
    --enable-shared \
    --disable-static \
    --enable-doc \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-ffprobe \
    --disable-ffserver \
    --disable-symver \
    --enable-avresample \
    --enable-small \
    --enable-gpl \
    --enable-libx264 \
    --enable-yasm \
    --extra-cflags="-I/Users/glumes/x264/android/arm/include" \
    --extra-ldflags="-L/Users/glumes/x264/android/arm/lib" \
    --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
    --target-os=linux \
    --arch=arm \
    --enable-cross-compile \
    --sysroot=$SYSROOT \
    --extra-cflags="-Os -fpic $ADDI_CFLAGS" \
    --extra-ldflags="$ADDI_LDFLAGS" \
    $ADDITIONAL_CONFIGURE_FLAG
make clean
make
make install
}

build_arm