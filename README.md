# GowEngine
A basic no nonsense LGPL-licensed portable C++ 2D game engine

## Supported Platforms
* Android
* iOS
* Linux
* macOS
* Windows

## Building
See [GowEngineMount](https://github.com/sgowen/GowEngineMount)

## Development Plan

### GowEngine
* Written in C++
* Uses OpenGL 2.0 and ES 2.0 rendering
* Dependencies:
    * glfw for windowing, input, and awesomeness on macOS, Linux, and Windows
    * ios-cmake for compiling libsndfile and openal-soft for iOS and macOS
    * libsndfile for loading sound files
    * openal-soft for audio rendering
    * rapidjson for json parsing
    * stb for image loading
* Will eventually:
    * be configured to use CMake build system
    * feature a memory tracker (using malloc with placement new operator, zero unique/shared pointers)
    * add support for latest OpenGL(ES) rendering
    * add support for Vulkan rendering
    * add support for Nintendo Switch
