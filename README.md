# GowEngine
A basic no nonsense LGPL-licensed portable C++ 2D game engine, how bout that?

## Supported Platforms
* Android
* iOS
* Linux
* macOS
* Windows

## Building
See [GowEngineMount](https://github.com/sgowen/GowEngineMount_nosfura2)

## Development Plan

### GowEngine
* Written in C++
* Uses OpenGL 2.0 and ES 3.0 rendering
* Dependencies:
    * glfw for windowing, input, and awesomeness on macOS, Linux, and Windows
    * ios-cmake
    * libsndfile
    * openal-soft
    * rapidjson for json parsing on all platforms
    * stb for image loading on all platforms
* Will eventually:
    * be configured to use CMake build system
    * feature a memory tracker (using malloc with placement new operator, zero unique/shared pointers)
    * add support for latest OpenGL(ES) rendering
    * add support for Vulkan rendering
    * add support for Nintendo Switch
