# GowEngine
A basic no nonsense LGPL-licensed portable C++ 2D game engine, how bout that?
Making games shouldn't cost money.
A solid idea manifested from within you is all you need, and you can't pay for that even if you wanted to.
If you don't have a solid idea, then does it really matter if you've paid the license fee for a robust game engine?
Nope. It's not like some game engine is going to come along and put the ideas in your head. That'd be... unreal.
And so, knowing this, why would I keep this repository closed source?
Take it. Use it. Build upon it.
This engine is nothing more than a collection of grunt work.
The goal here is to remove the barriers, empowering you to make your game, all with zero budget.
Because if you have a solid idea, I WANT to see it fully realized.
I'm not going to expect you to somehow have a bunch of money to throw around too.
And even if you did, it's clear that $$$ does not equal premium content (looking at you FF7R).
Let's cultivate an environment where the best ideas win, and then we will all win.

## Supported Platforms
* Android
* iOS
* Linux
* macOS
* Windows

## Building
See [GowEngineMount](https://github.com/sgowindie/GowEngineMount)

## Development Plan

### GowEngine (this repo)
* Written in C++
* Supports only OpenGL rendering
* Dependencies:
    * box2d is a 2D physics engine for games
    * DirectXTK for XAudio2 on Windows (no need for d3d lib though as XAudio2 is packaged with Windows 10)
    * glfw for windowing, input, and awesomeness on macOS, Linux, and Windows
    * ObjectAL-for-iPhone for OpenAL audio rendering on iOS and macOS
    * oboe for AAudio/OpenSL ES audio rendering on Android
    * rapidjson for json parsing on all platforms
    * spdlog for console/file logging on all platforms
    * stb for image loading on all platforms
* Will eventually:
    * be configured to use CMake build system
    * implement openal-soft
    * feature a memory tracker (using malloc with placement new operator, zero unique/shared pointers)
    * add support for Vulkan rendering
    * add support for Nintendo Switch
