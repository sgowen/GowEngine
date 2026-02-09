//
//  GlfwEngineMount.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/16/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_DESKTOP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <chrono>

using namespace std;
using namespace std::chrono;

Engine* _engine = nullptr;
bool _isMouseDown = false;
bool _isRightClick = false;

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

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    _engine->onCursorScrolled((float)yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x;
    double y;
    glfwGetCursorPos(window, &x, &y);
    
    _isRightClick = button == GLFW_MOUSE_BUTTON_2;
    
    switch (action)
    {
        case GLFW_PRESS:
            _engine->onCursorDown((float)x, (float)y, _isRightClick);
            _isMouseDown = true;
            break;
        case GLFW_RELEASE:
            _engine->onCursorUp((float)x, (float)y, _isRightClick);
            _isMouseDown = false;
            break;
        default:
            break;
    }
}

void mouse_cursor_pos_callback(GLFWwindow* window, double x, double y)
{
    if (_isMouseDown)
    {
        _engine->onCursorDragged((float)x, (float)y, _isRightClick);
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

void runEngine(Engine& engine, GLFWwindow* window, double deltaTime)
{
    for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_4; ++i)
    {
        bool isJoystickPresent = glfwJoystickPresent(i);
        bool isJoystickGamepad = glfwJoystickIsGamepad(i);
        
        if (!isJoystickPresent || !isJoystickGamepad)
        {
            continue;
        }
        
        GLFWgamepadstate state;
        if (glfwGetGamepadState(i, &state))
        {
            engine.onGamepadInputButton(i, GEB_BUTTON_A, state.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS);
            engine.onGamepadInputButton(i, GEB_BUTTON_B, state.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS);
            engine.onGamepadInputButton(i, GEB_BUTTON_X, state.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS);
            engine.onGamepadInputButton(i, GEB_BUTTON_Y, state.buttons[GLFW_GAMEPAD_BUTTON_Y] == GLFW_PRESS);
            
            engine.onGamepadInputButton(i, GEB_BUMPER_LEFT, state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS);
            engine.onGamepadInputButton(i, GEB_BUMPER_RIGHT, state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS);
            
            engine.onGamepadInputButton(i, GEB_BUTTON_START, state.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_PRESS);
            engine.onGamepadInputButton(i, GEB_BUTTON_BACK, state.buttons[GLFW_GAMEPAD_BUTTON_BACK] == GLFW_PRESS);
            engine.onGamepadInputButton(i, GEB_BUTTON_GUIDE, state.buttons[GLFW_GAMEPAD_BUTTON_GUIDE] == GLFW_PRESS);
            
            engine.onGamepadInputButton(i, GEB_D_PAD_UP, state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS);
            engine.onGamepadInputButton(i, GEB_D_PAD_DOWN, state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS);
            engine.onGamepadInputButton(i, GEB_D_PAD_LEFT, state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS);
            engine.onGamepadInputButton(i, GEB_D_PAD_RIGHT, state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS);
            
            engine.onGamepadInputStickLeft(i, state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
            engine.onGamepadInputStickRight(i, state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
            engine.onGamepadInputTrigger(i, state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER], state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]);
        }
    }

    EngineRequestedHostAction requestedAction = ERHA_DEFAULT;
    if (ENGINE_CFG.glfwLoggingEnabled())
    {
        auto updateStart = high_resolution_clock::now();
        requestedAction = engine.update(deltaTime);
        auto updateStop = high_resolution_clock::now();
        auto updateDur = duration_cast<microseconds>(updateStop - updateStart);
        LOG("UPDATE took: %d microseconds", updateDur.count());
    }
    else
    {
        requestedAction = engine.update(deltaTime);
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
    
    if (engine.hasUpdatedSinceLastRender())
    {
        if (ENGINE_CFG.glfwLoggingEnabled())
        {
            auto renderStart = high_resolution_clock::now();
            engine.render();
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
            engine.render();
            glfwSwapBuffers(window);
        }
    }
}

void printNetInfo()
{
    char hostbuffer[256];
    struct hostent *host_entry;
    int hostname;
    struct in_addr **addr_list;

    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    if (hostname == -1)
    {
        LOG("gethostname error");
        return;
    }
    LOG("Hostname: %s\n", hostbuffer);

    host_entry = gethostbyname(hostbuffer);
    if (host_entry == NULL)
    {
        LOG("gethostbyname error");
        return;
    }
    
    addr_list = (struct in_addr **)host_entry->h_addr_list;
    for (int i = 0; addr_list[i] != NULL; i++)
    {
        LOG("IP address %d: %s\n", i+1, inet_ntoa(*addr_list[i]));
    }
}

void updateGamepadMappings()
{
    std::string gamecontrollerdbFilePath = "engine/gamecontrollerdb.txt";
    FileData mappingsFileData = ASSET_HANDLER.loadAsset(gamecontrollerdbFilePath);
    const char* mappings = (const char*)mappingsFileData._data;
    if (glfwUpdateGamepadMappings(mappings))
    {
        LOG("Gamepad mappings updated successfully.");
    }
    else
    {
        LOG("Failed to update gamepad mappings.");
    }
    ASSET_HANDLER.unloadAsset(mappingsFileData);
}

int main(int argc, char *argv[])
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    
    // TODO, kind of weird to instantiate Engine here.
    // But we have to because otherwise ENGINE_CFG will be null
    // Consider loading config here
    // And instanting Engine only inside runEngine function
    Engine engine;
    
    printNetInfo();
    updateGamepadMappings();

    GLFWmonitor* monitor = nullptr;

    int width = 1024;
    int height = 576;

    if (ENGINE_CFG.fullScreen())
    {
        // TODO, this isn't working on macOS
        monitor = glfwGetPrimaryMonitor();
        if (monitor)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);

            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            width = mode->width;
            height = mode->height;
        }
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    window = glfwCreateWindow(width, height, ENGINE_CFG.title().c_str(), monitor, nullptr);
    if (window == nullptr)
    {
        LOG("window is null, terminating...");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    INST_REG.registerInstance(window, "GLFWwindow");

    glfwSetWindowIconifyCallback(window, window_iconify_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_cursor_pos_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    
    OGL.loadOpenGL(glfwGetProcAddress);
    
    _engine = &engine;
    
    GlfwClipboardHandler clipboardHandler(window);
    engine.createDeviceDependentResources(&clipboardHandler);
    
    glfwSwapInterval(ENGINE_CFG.vsync() ? 1 : 0);
    glfwSetTime(0.0);

    double lastTime = glfwGetTime();
    int lastKnownWidth = 0;
    int lastKnownHeight = 0;
    
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
            
            engine.onWindowSizeChanged(width, height, screenWidth, screenHeight);

            lastKnownWidth = width;
            lastKnownHeight = height;
        }

        runEngine(engine, window, deltaTime);
        
        if (ENGINE_CFG.glfwLoggingEnabled())
        {
            auto frameStop = high_resolution_clock::now();
            auto frameDur = duration_cast<microseconds>(frameStop - frameStart);
            
            LOG("FRAME took: %d microseconds", frameDur.count());
        }
    }

    engine.destroyDeviceDependentResources();

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}

#endif /* IS_DESKTOP */
