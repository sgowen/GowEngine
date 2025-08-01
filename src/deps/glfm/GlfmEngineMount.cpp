//
//  GlfmEngineMount.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/19/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_MOBILE

#include "glfm.h"

static Engine *_engine = nullptr;
static double lastTime = 0.0;
static int lastWidth = 0;

static void onSurfaceCreated(GLFMDisplay *display, int width, int height)
{
    UNUSED(display);
    UNUSED(width);
    UNUSED(height);

    if (_engine != nullptr)
    {
        _engine->destroyDeviceDependentResources();
        delete _engine;
        _engine = nullptr;
    }
    
#if IS_ANDROID
    // TODO, is there a better way to set this?
    FileCompatUtil::androidActivity = static_cast<ANativeActivity *>(glfmGetAndroidActivity(display));
#endif
}

static void onSurfaceDestroyed(GLFMDisplay *display)
{
    UNUSED(display);

    if (_engine != nullptr)
    {
        _engine->destroyDeviceDependentResources();
        delete _engine;
        _engine = nullptr;
    }
}

static void onDraw(GLFMDisplay *display)
{
    if (_engine == nullptr)
    {
        lastTime = glfmGetTime();
        _engine = new Engine();
        _engine->createDeviceDependentResources();
    }

    int width, height;
    glfmGetDisplaySize(display, &width, &height);
    if (lastWidth != width)
    {
        lastWidth = width;
        _engine->onWindowSizeChanged(width, height, width, height);
    }

    double timeNow = glfmGetTime();
    double deltaTime = timeNow - lastTime;
    lastTime = timeNow;
    _engine->update(deltaTime);
    
    if (_engine->hasUpdatedSinceLastRender())
    {
        _engine->render();
        glfmSwapBuffers(display);
    }
}

static bool onTouch(GLFMDisplay *display, int touch, GLFMTouchPhase phase, double x, double y)
{
    if (phase == GLFMTouchPhaseBegan)
    {
        _engine->onCursorDown(x, y);
        return true;
    }
    else if (phase == GLFMTouchPhaseMoved)
    {
        _engine->onCursorDragged(x, y);
        return true;
    }
    else if (phase == GLFMTouchPhaseEnded ||
             phase == GLFMTouchPhaseCancelled)
    {
        _engine->onCursorUp(x, y);
        return true;
    }
    else
    {
        return false;
    }
}

static bool onKey(GLFMDisplay *display, GLFMKeyCode keyCode, GLFMKeyAction action, int modifiers)
{
    _engine->onKeyboardInput(keyCode, action == GLFMKeyActionReleased);
    return true;
}

void glfmMain(GLFMDisplay *display)
{
    glfmSetDisplayChrome(display, GLFMUserInterfaceChromeNone);
    glfmSetDisplayConfig(
            display,
            GLFMRenderingAPIOpenGLES2,
            GLFMColorFormatRGBA8888,
            GLFMDepthFormatNone,
            GLFMStencilFormatNone,
            GLFMMultisampleNone);
    glfmSetSurfaceCreatedFunc(display, onSurfaceCreated);
    glfmSetSurfaceDestroyedFunc(display, onSurfaceDestroyed);
    glfmSetRenderFunc(display, onDraw);
    glfmSetMultitouchEnabled(display, true);
    glfmSetTouchFunc(display, onTouch);
    glfmSetKeyFunc(display, onKey);
}

#endif /* IS_MOBILE */
