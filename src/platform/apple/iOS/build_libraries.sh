#!/bin/sh

cd ${SRCROOT}
cd iOS

libbox2d=1
if [[ (-e libbox2d.a) ]]; then
    echo 'libbox2d.a already exists'
    libbox2d=0
fi

libglfm=1
if [[ (-e libglfm.a) ]]; then
    echo 'libglfm.a already exists'
    libglfm=0
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
    cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../../ios-cmake/ios.toolchain.cmake -DPLATFORM=OS64 -DDEPLOYMENT_TARGET="12.0"
    cmake --build . --config Release --target box2d
    cp bin/Release/libbox2d.a ${SRCROOT}/iOS/libbox2d.a
    cd ..
    rm -rf cmake_build
    cd ..
fi

if [[ "$libglfm" == 1 ]]; then
    echo 'building libglfm'
    cd glfm
    rm -rf cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../../ios-cmake/ios.toolchain.cmake -DPLATFORM=OS64 -DDEPLOYMENT_TARGET="12.0" -D GLFM_BUILD_EXAMPLES=OFF
    cmake .. -D GLFM_BUILD_EXAMPLES=OFF -B build/apple -G Xcode
    cd build/apple
    cmake --build . --config Release --target glfm
    cp GLFM.build/lib/Release/libglfm.a ${SRCROOT}/iOS/
    cd ../../..
    rm -rf cmake_build
    cd ..
fi

if [[ "$libsndfile" == 1 ]]; then
    echo 'building libsndfile'
    cd libsndfile
    mv CMakeLists.txt CMakeLists.txt.bak
    cp ${SRCROOT}/iOS/CMakeLists_libsndfile.txt CMakeLists.txt
    rm -rf cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../../ios-cmake/ios.toolchain.cmake -DPLATFORM=OS64 -DDEPLOYMENT_TARGET="12.0" -DBUILD_SHARED_LIBS=1 -DBUILD_TESTING=0 -DENABLE_PACKAGE_CONFIG=0
    cmake --build . --config Release --target sndfile
    cp Release-iphoneos/libsndfile.1.0.35.dylib ${SRCROOT}/iOS/libsndfile.1.dylib
    cd ..
    mv CMakeLists.txt.bak CMakeLists.txt
    rm -rf cmake_build
    cd ..
fi

if [[ "$libopenal" == 1 ]]; then
    echo 'building libopenal'
    cd openal-soft
    rm -rf cmake_build
    mkdir cmake_build
    cd cmake_build
    cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../../ios-cmake/ios.toolchain.cmake -DPLATFORM=OS64 -DDEPLOYMENT_TARGET="12.0"
    cmake --build . --config Release --target OpenAL
    cp Release-iphoneos/libopenal.1.23.1.dylib ${SRCROOT}/iOS/libopenal.1.dylib
    cd ..
    rm -rf cmake_build
    cd ..
fi
