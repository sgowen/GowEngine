# GowEngine
An easy to use, easy to read, no nonsense LGPL 2.1 licensed portable C++ 2D game engine

## Demo
![Demo](https://github.com/sgowen/GowEngine/blob/main/demo.gif)

## Features
* Written in portable C++17
* Graphics rendered with OpenGL 2.0 and OpenGL ES 2.0
* Audio rendered with OpenAL
* Real-time multiplayer networking with UDP sockets
* Data Driven Architecture with JSON and Lua
* Homegrown physics and Box2D physics
* All LGPL licensed dependencies are compiled into shared libraries that can be swapped out by end users, cuz that's how that's supposed to work, right?

## Supported Platforms
* Android
* iOS
* Linux
* macOS
* Windows

## Dependencies
* [bin_to_c](https://github.com/sgowen/bin_to_c): converts files to binary for embedding inside GowEngine
* [box2d](https://github.com/erincatto/box2d): handles physics (optional)
* [file-compat](https://github.com/brackeen/file-compat): implements fopen for Android and Windows  
* [glfm](https://github.com/brackeen/glfm): creates window and OpenGL context and handles input on Android and iOS
* [glfw](https://github.com/glfw/glfw): creates window and OpenGL context and handles input on Linux, macOS, and Windows
* [imgui](https://github.com/ocornut/imgui): handles user interface
* [ios-cmake](https://github.com/leetal/ios-cmake): handles cmake stuff for iOS and macOS
* [libsndfile](https://github.com/libsndfile/libsndfile): loads sounds
* [Lua](https://github.com/walterschell/Lua): convenient CMake project for buidling Lua 5.4.6
* [openal-soft](https://github.com/kcat/openal-soft): renders audio
* [rapidjson](https://github.com/Tencent/rapidjson): loads json
* [sol2](https://github.com/ThePhD/sol2): an incredible C++ Lua API wrapper
* [stb](https://github.com/nothings/stb): loads textures
* [steamworks_sdk](https://github.com/sgowen/steamworks_sdk): integrates with Steam (optional)

## Building
* [src/platform/android](https://github.com/sgowen/GowEngine/tree/main/src/platform/android): Simply open this folder in Android Studio to compile/run on an Android device
* [src/platform/apple](https://github.com/sgowen/GowEngine/tree/main/src/platform/apple): Open with Xcode to compile/run on a Mac or an iPhone
* [src/platform/linux](https://github.com/sgowen/GowEngine/tree/main/src/platform/linux): Open with Code::Blocks to compile/run in a Linux environment
* [src/platform/windows](https://github.com/sgowen/GowEngine/tree/main/src/platform/windows): Open with Visual Studio (currently Version 17.1.5) to compile/run on Windows

## Roadmap
* Implement a memory tracker via logging calls to malloc with placement new operator (currently no memory leaks, but if ever I encounter them again, I need to be better equipped for that battle)
