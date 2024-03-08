//
//  IMGUIEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void IMGUIEngineState::onEnter(Engine* e)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(INST_REG.get<GLFWwindow>("GLFWwindow"), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 110");
}

void IMGUIEngineState::onAssetsLoaded(Engine *e)
{
    // Empty
}

void IMGUIEngineState::onExit(Engine* e)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void IMGUIEngineState::onUpdate(Engine* e)
{
    uint8_t inputState = _inputProcessor.update();
    switch (inputState)
    {
        case IPS_EXIT:
            e->popState();
            break;
    }
}

void IMGUIEngineState::onRender(Renderer& r)
{
    r.bindFramebuffer("main");
    r.renderToScreen();
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

IMGUIEngineState::IMGUIEngineState() : EngineState("json/game/Config.json"),
_inputProcessor()
{
    // Empty
}
