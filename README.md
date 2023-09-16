# GowEngine
A basic no nonsense LGPL 2.1 licensed portable C++ 2D game engine

## Demo
![Demo](https://github.com/sgowen/GowEngine/blob/main/demo.gif)

## Features
* Written in portable C++11
* Graphics rendered with OpenGL 2.0 and OpenGL ES 2.0
* Audio rendered with OpenAL
* Real-time multiplayer networking with UDP sockets
* JSON data-driven architecture
* Basic Physics
* All LGPL licensed dependencies are compiled into shared libraries that can be swapped out by end users, cuz that's how that's supposed to work, right?

## Supported Platforms
* Android
* iOS
* Linux
* macOS
* Windows
* Soon... Nintendo DS!

## Dependencies
* [glfw](https://github.com/glfw/glfw): creates window and OpenGL context and handles input on macOS, Linux, and Windows
* [ios-cmake](https://github.com/leetal/ios-cmake): handles cmake stuff for iOS and macOS
* [libsndfile](https://github.com/libsndfile/libsndfile): loads sounds
* [openal-soft](https://github.com/kcat/openal-soft): renders audio
* [rapidjson](https://github.com/Tencent/rapidjson): loads json
* [stb](https://github.com/nothings/stb): loads textures

## Building
* Template projects for various platforms reside in [GowEngineMount](https://github.com/sgowen/GowEngineMount), simply clone it into the same directory as GowEngine side by side

## Roadmap
* Implement a memory tracker via logging calls to malloc with placement new operator (currently no memory leaks, but if ever I encounter them again, I need to be better equipped for that battle)
