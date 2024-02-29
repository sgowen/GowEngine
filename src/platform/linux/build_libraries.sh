#!/bin/bash

solutionDir=$(pwd)

libbox2d=1
if [[ -f "libbox2d.a" ]]; then
    echo 'libbox2d.a already exists'
    libbox2d=0
fi

libglfw3=1
if [[ -f "libglfw3.a" ]]; then
    echo 'libglfw3.a already exists'
    libglfw3=0
fi

libsndfile=1
if [[ -f "libsndfile.so.1.0.35" ]]; then
    echo 'libsndfile.so.1.0.35 already exists'
    libsndfile=0
fi

libopenal=1
if [[ -f "libopenal.so.1" ]]; then
    echo 'libopenal.so.1 already exists'
    libopenal=0
fi

cd ../../../deps

if [[ "$libbox2d" == 1 ]]; then
    echo 'building libbox2d'
    cd box2d
    rm -rf cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake ..
    cmake --build . --config Release --target box2d
    cp bin/libbox2d.a $solutionDir/
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
    cmake ..
    cmake --build . --config Release --target glfw
    cp src/libglfw3.a $solutionDir/
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
    cmake .. -DBUILD_SHARED_LIBS=1 -DBUILD_TESTING=0
    cmake --build . --config Release --target sndfile
    cp -P libsndfile.so $solutionDir/
    cp -P libsndfile.so.1 $solutionDir/
    cp libsndfile.so.1.0.35 $solutionDir/
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
    cmake ..
    cmake --build . --config Release --target OpenAL
    cp -P libopenal.so $solutionDir/
    cp libopenal.so.1.23.1 $solutionDir/libopenal.so.1
    cd ..
    rm -rf cmake_build
    cd ..
fi

echo 'copying libsteam_api.so into solution directory'
cp steamworks_sdk/redistributable_bin/linux64/libsteam_api.so $solutionDir/
