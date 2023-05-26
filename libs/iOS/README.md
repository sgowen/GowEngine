# iOS libs

Need to build the following and place in this directory:
* libsndfile.a
* libopenal.1.dylib

Build Command (run from this iOS directory):
```bash
LIBS_DIR=$(pwd) \
&& cd ../../deps \
&& cd libsndfile \
&& sed -i -e 's/option (ENABLE_PACKAGE_CONFIG "Generate and install package config file" ON)/option (ENABLE_PACKAGE_CONFIG "Generate and install package config file" OFF)/' CMakeLists.txt \
&& rm -rf cmake_build \
&& mkdir cmake_build \
&& cd cmake_build \
&& cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../../ios-cmake/ios.toolchain.cmake -DPLATFORM=OS64 -DDEPLOYMENT_TARGET="12.0" \
&& cmake --build . --config Release --target sndfile \
&& cp Release-iphoneos/libsndfile.a $LIBS_DIR/libsndfile.a \
&& cd .. \
&& mv CMakeLists.txt-e CMakeLists.txt \
&& rm -rf cmake_build \
&& cd .. \
&& cd openal-soft \
&& sed -i -e '/IOAudioTypes.h/d' alc/backends/coreaudio.cpp \
&& rm -rf cmake_build \
&& mkdir cmake_build \
&& cd cmake_build \
&& cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../../ios-cmake/ios.toolchain.cmake -DPLATFORM=OS64 -DDEPLOYMENT_TARGET="12.0" \
&& cmake --build . --config Release --target OpenAL \
&& cp Release-iphoneos/libopenal.1.23.1.dylib $LIBS_DIR/libopenal.1.dylib \
&& cd .. \
&& mv alc/backends/coreaudio.cpp-e alc/backends/coreaudio.cpp \
&& rm -rf cmake_build \
&& cd $LIBS_DIR \
&& ls
```

Useful Links:
* https://github.com/leetal/ios-cmake
* https://github.com/lloyd/yajl/issues/201
