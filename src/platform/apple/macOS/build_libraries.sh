#!/bin/sh

cd ${SRCROOT}
cd macOS

libbox2d=1
if [[ (-e libbox2d.a) ]]; then
    echo 'libbox2d.a already exists'
    libbox2d=0
fi

libglfw3=1
if [[ (-e libglfw3.a) ]]; then
    echo 'libglfw3.a already exists'
    libglfw3=0
fi

libsndfile=1
if [[ (-e libsndfile.1.dylib) ]]; then
    echo 'libsndfile.1.dylib already exists'
    libsndfile=0
fi

libopenal=1
if [[ (-e libopenal.1.dylib) ]]; then
    echo 'libopenal.1.dylib already exists'
    libopenal=0
fi

cd ../../../../deps

if [[ "$libbox2d" == 1 ]]; then
    echo 'building libbox2d'
    cd box2d
    rm -rf cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../../ios-cmake/ios.toolchain.cmake -DPLATFORM=MAC
    cmake --build . --config Release --target box2d
    cp bin/Release/libbox2d.a ${SRCROOT}/macOS/libbox2d.a
    cd ..
    rm -rf cmake_build
    cd ..
fi

if [[ "$libglfw3" == 1 ]]; then
    echo 'building libglfw3'
    cd glfw
    rm -rf cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake .. -G Xcode
    cmake --build . --config Release --target glfw
    cp src/Release/libglfw3.a ${SRCROOT}/macOS/
    cd ..
    rm -rf cmake_build
    cd ..
fi

if [[ "$libsndfile" == 1 ]]; then
    echo 'building libsndfile'
    cd libsndfile
    rm -rf cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../../ios-cmake/ios.toolchain.cmake -DPLATFORM=MAC -DBUILD_SHARED_LIBS=1 -DBUILD_TESTING=0 -DENABLE_PACKAGE_CONFIG=0
    cmake --build . --config Release --target sndfile
    cp Release/libsndfile.1.0.35.dylib ${SRCROOT}/macOS/libsndfile.1.dylib
    cd ..
    rm -rf cmake_build
    cd ..
fi

if [[ "$libopenal" == 1 ]]; then
    echo 'building libopenal'
    cd openal-soft
    rm -rf cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../../ios-cmake/ios.toolchain.cmake -DPLATFORM=MAC
    cmake --build . --config Release --target OpenAL
    cp Release/libopenal.1.23.1.dylib ${SRCROOT}/macOS/libopenal.1.dylib
    cd ..
    rm -rf cmake_build
    cd ..
fi
