//
//  GlfwEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/16/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_DESKTOP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <chrono>

using namespace std;
using namespace std::chrono;

Engine* _engine = nullptr;
int joysticks[GLFW_JOYSTICK_LAST + 1];
int joystick_count = 0;
bool isDown = false;
bool isAlt = false;

void error_callback(int error, const char* description)
{
    LOG("GLFW Error: %s", description);
}

void window_iconify_callback(GLFWwindow* window, int iconified)
{
    if (iconified)
    {
        _engine->onPause();
    }
    else
    {
        _engine->onResume();
    }
}

void joystick_callback(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {
        joysticks[joystick_count++] = jid;
    }
    else if (event == GLFW_DISCONNECTED)
    {
        int i;

        for (i = 0; i < joystick_count; ++i)
        {
            if (joysticks[i] == jid)
            {
                break;
            }
        }

        for (i = i + 1; i < joystick_count; ++i)
        {
            joysticks[i - 1] = joysticks[i];
        }

        joystick_count--;
    }
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    _engine->onCursorScrolled((float)yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x;
    double y;
    glfwGetCursorPos(window, &x, &y);
    
    isAlt = button == GLFW_MOUSE_BUTTON_2;
    
    switch (action)
    {
        case GLFW_PRESS:
            _engine->onCursorDown((float)x, (float)y, isAlt);
            isDown = true;
            break;
        case GLFW_RELEASE:
            _engine->onCursorUp((float)x, (float)y, isAlt);
            isDown = false;
            break;
        default:
            break;
    }
}

void mouse_cursor_pos_callback(GLFWwindow* window, double x, double y)
{
    if (isDown)
    {
        _engine->onCursorDragged((float)x, (float)y, isAlt);
    }
    else
    {
        _engine->onCursorMoved((float)x, (float)y);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    _engine->onKeyboardInput(key, action == GLFW_RELEASE);
}

class GlfwClipboardHandler : public ClipboardHandler
{
public:
    virtual const char* getClipboardString()
    {
        return glfwGetClipboardString(_window);
    }
    
    GlfwClipboardHandler(GLFWwindow* window) :
    _window(window)
    {
        // Empty
    }
    virtual ~GlfwClipboardHandler() {}
    
private:
    GLFWwindow* _window;
};

void runEngine(Engine* engine, GLFWwindow* window)
{
    OGL.loadOpenGL(glfwGetProcAddress);
    
    _engine = engine;
    
    GlfwClipboardHandler clipboardHandler(window);
    engine->createDeviceDependentResources(&clipboardHandler);
    
    glfwSwapInterval(ENGINE_CFG.glfwSwapInterval());
    glfwSetTime(0.0);

    double lastTime = glfwGetTime();
    int lastKnownWidth = 0, lastKnownHeight = 0;
    int width = 0, height = 0;
    
    while (!glfwWindowShouldClose(window))
    {
        auto frameStart = high_resolution_clock::now();
        
        double timeNow = glfwGetTime();
        double deltaTime = timeNow - lastTime;
        lastTime = timeNow;
        
        glfwPollEvents();
        
        glfwGetFramebufferSize(window, &width, &height);
        if (width != lastKnownWidth || height != lastKnownHeight)
        {
            int screenWidth = 0;
            int screenHeight = 0;
            glfwGetWindowSize(window, &screenWidth, &screenHeight);
            
            engine->onWindowSizeChanged(width, height, screenWidth, screenHeight);

            lastKnownWidth = width;
            lastKnownHeight = height;
        }

        for (int i = 0; i < joystick_count; ++i)
        {
            int j, axis_count, button_count;
            const float* axes;
            const uint8_t* buttons;

            axes = glfwGetJoystickAxes(joysticks[i], &axis_count);
            buttons = glfwGetJoystickButtons(joysticks[i], &button_count);

            float stickLeftX = 0;
            float stickLeftY = 0;
            float stickRightX = 0;
            float stickRightY = 0;
            float triggerLeft = 0;
            float triggerRight = 0;

            for (j = 0; j < axis_count; ++j)
            {
                switch (j)
                {
                    case 0:
                        stickLeftX = axes[j];
                        break;
                    case 1:
                        stickLeftY = axes[j];
                        break;
                    case 2:
                        stickRightX = axes[j];
                        break;
                    case 3:
                        triggerLeft = axes[j];
                        break;
                    case 4:
                        triggerRight = axes[j];
                        break;
                    case 5:
                        stickRightY = axes[j];
                    default:
                        break;
                }
            }

            engine->onGamepadInputStickLeft(i, stickLeftX, stickLeftY);
            engine->onGamepadInputStickRight(i, stickRightX, stickRightY);
            engine->onGamepadInputTrigger(i, triggerLeft, triggerRight);

            for (j = 0; j < button_count; ++j)
            {
                engine->onGamepadInputButton(i, j, buttons[j]);
            }
        }

        EngineRequestedHostAction requestedAction = ERHA_DEFAULT;
        if (ENGINE_CFG.glfwLoggingEnabled())
        {
            auto updateStart = high_resolution_clock::now();
            requestedAction = engine->update(deltaTime);
            auto updateStop = high_resolution_clock::now();
            auto updateDur = duration_cast<microseconds>(updateStop - updateStart);
            LOG("UPDATE took: %d microseconds", updateDur.count());
        }
        else
        {
            requestedAction = engine->update(deltaTime);
        }
        
        switch (requestedAction)
        {
            case ERHA_EXIT:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            case ERHA_DEFAULT:
            default:
                break;
        }
        
        if (ENGINE_CFG.glfwLoggingEnabled())
        {
            auto renderStart = high_resolution_clock::now();
            engine->render();
            auto renderStop = high_resolution_clock::now();
            auto renderDur = duration_cast<microseconds>(renderStop - renderStart);
            LOG("RENDER took: %d microseconds", renderDur.count());
            
            auto glfwSwapBuffersStart = high_resolution_clock::now();
            glfwSwapBuffers(window);
            auto glfwSwapBuffersStop = high_resolution_clock::now();
            auto glfwSwapBuffersDur = duration_cast<microseconds>(glfwSwapBuffersStop - glfwSwapBuffersStart);
            LOG("glfwSwapBuffers took: %d microseconds", glfwSwapBuffersDur.count());
        }
        else
        {
            engine->render();
            glfwSwapBuffers(window);
        }
        
        auto frameStop = high_resolution_clock::now();
        auto frameDur = duration_cast<microseconds>(frameStop - frameStart);
        
        if (ENGINE_CFG.glfwLoggingEnabled())
        {
            LOG("FRAME took: %d microseconds", frameDur.count());
        }
    }

    engine->destroyDeviceDependentResources();
}

void GlfwEngine::exec(Engine* engine, const char* windowTitle)
{
    memset(joysticks, 0, sizeof(joysticks));

    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWmonitor* monitor = nullptr;

    int width = 800;
    int height = 480;

#if IS_RELEASE
//    monitor = glfwGetPrimaryMonitor();
//    if (monitor)
//    {
//        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
//
//        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
//        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
//        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
//        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
//
//        width = mode->width;
//        height = mode->height;
//    }
#endif

    window = glfwCreateWindow(width, height, windowTitle, monitor, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; ++jid)
    {
        if (glfwJoystickPresent(jid))
        {
            joysticks[joystick_count++] = jid;
        }
    }

    glfwSetWindowIconifyCallback(window, window_iconify_callback);
    glfwSetJoystickCallback(joystick_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_cursor_pos_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    
    runEngine(engine, window);
    
    delete engine;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

#endif /* IS_DESKTOP */
