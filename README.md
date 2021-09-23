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

## Development Plan

### GowEngine (this repo)
* C++ only core (no resource files)
* Platform specific code okay if it's reusable (e.g. AssetHandlers) and compiled selectively
* Initially only supports OpenGL rendering, but will also include Vulkan
* Audio support is platform specific with a platform agnostic API
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
    * be configured to use CMake build system (but for now is actively developed as part of GowEngineDemo)
    * Implement OpenAL Soft and provide its dynamic libraries for all platforms (with the lib being easily replaceable as per LGPL requirements)
    * feature a memory tracker (using malloc with placement new operator, zero unique/shared pointers)
    * feature in-house developed vector, map, deque classes (no STL needed!)
    * Add support for Vulkan rendering
    * Add support for Nintendo Switch

### GowEngineDemo (separate repo)
* Features example projects for all supported platforms
* Execute parameterized script to create new game project using this repo as the template

### Game specific code (resides in its own repo apart from GowEngine)
* Initializes Engine with:
    * desired framesPerSecond (e.g. 30, 60)
    * initial state (Engine uses state machine)
    * cursor/keyboard/gamepad input events received from platform layer
* Contains resources for game (shaders, textures, audio, json, etc.)
* This is where the game logic goes (e.g. world/level design, scene rendering logic)

### Platform projects (all stuffed into game repo)
* This is where stuff like GLSurfaceView, GLKViewController, glfw goes
* Necessary JNI/Swift C++ bridging code goes here
* After window/input/EGL init, initialize Game code with:
    * window size
    * cursor/keyboard/gamepad input events
