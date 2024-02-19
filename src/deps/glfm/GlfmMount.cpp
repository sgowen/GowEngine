//
//  GlfmMount.cpp
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
}

static void onSurfaceDestroyed(GLFMDisplay *display)
{
    UNUSED(display);

    _engine->destroyDeviceDependentResources();
    delete _engine;
    _engine = nullptr;
}

static void onDraw(GLFMDisplay *display)
{
    if (_engine == nullptr)
    {
        lastTime = glfmGetTime();
        FileUtil::androidActivity = static_cast<ANativeActivity *>(glfmGetAndroidActivity(display));
        _engine = GowEngineMount::createEngine();
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
    _engine->render();

    glfmSwapBuffers(display);
}

void glfmMain(GLFMDisplay *display)
{
    glfmSetDisplayConfig(
            display,
            GLFMRenderingAPIOpenGLES2,
            GLFMColorFormatRGBA8888,
            GLFMDepthFormatNone,
            GLFMStencilFormatNone,
            GLFMMultisampleNone);
    glfmSetSurfaceCreatedFunc(display, onSurfaceCreated);
    glfmSetRenderFunc(display, onDraw);
    glfmSetSurfaceDestroyedFunc(display, onSurfaceDestroyed);
}

#endif /* IS_MOBILE */
