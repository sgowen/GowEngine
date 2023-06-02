# GowEngine
A basic no nonsense LGPL 2.1 licensed portable C++ 2D game engine

## Basics
* Written in portable C++11
* Graphics rendered with OpenGL 2.0 and OpenGL ES 2.0
* Audio rendered with OpenAL
* All LGPL 2.1 licensed dependencies are compiled into shared libraries that can be swapped out by end users

## Supported Platforms
* Android
* iOS
* Linux
* macOS
* Windows

## Dependencies
* **glfw**: windowing, input, and general awesomeness on macOS, Linux, and Windows
* **ios-cmake**: compiling libsndfile and openal-soft for iOS and macOS
* **libsndfile**: loading sound files
* **openal-soft**: audio rendering
* **rapidjson**: loading json files
* **stb**: loading textures

## Building
See [GowEngineMount](https://github.com/sgowen/GowEngineMount)

## Development Plan
* Configure to use CMake
* Implement a memory tracker via logging calls to malloc with placement new operator
