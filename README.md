# GowEngine
A basic no nonsense LGPL 2.1 licensed portable C++ 2D game engine

## Demo
![Demo](https://file.io/Isa1VANXnuVu)

## Features
* Written in portable C++11
* Graphics rendered with OpenGL 2.0 and OpenGL ES 2.0
* Audio rendered with OpenAL
* Real-time multiplayer networking with UDP sockets
* JSON data-driven architecture
* Basic Physics
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
* Template projects for various platforms reside in [GowEngineMount](https://github.com/sgowen/GowEngineMount), simply clone it into the same directory as GowEngine side by side

## Roadmap
* Configure to use CMake
* Implement a memory tracker via logging calls to malloc with placement new operator
