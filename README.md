# GowEngine
A basic no nonsense LGPL 2.1 licensed portable C++ 2D game engine

## Demo
![Demo](https://file.io/Isa1VANXnuVu)

## Basics
* Written in portable C++11
* Graphics rendered with OpenGL 2.0 and OpenGL ES 2.0
* Audio rendered with OpenAL
* All LGPL licensed dependencies are compiled into shared libraries that can be swapped out by end users

## Supported Platforms
* Android
* iOS
* Linux
* macOS
* Windows

## Dependencies
* [glfw](https://github.com/glfw/glfw): windowing, input, and general awesomeness on macOS, Linux, and Windows
* [ios-cmake](https://github.com/leetal/ios-cmake): compiling libsndfile and openal-soft for iOS and macOS
* [libsndfile](https://github.com/libsndfile/libsndfile): loading sound files
* [openal-soft](https://github.com/kcat/openal-soft): audio rendering
* [rapidjson](https://github.com/Tencent/rapidjson): loading json files
* [stb](https://github.com/nothings/stb): loading textures

## Building
* Template projects for various platforms in [GowEngineMount](https://github.com/sgowen/GowEngineMount), clone into the same directory as GowEngine side by side
* Regardless of target platform, CMake and Python must be installed in order to build GowEngine's dependencies

## Development Plan
* Configure to use CMake
* Implement a memory tracker via logging calls to malloc with placement new operator
