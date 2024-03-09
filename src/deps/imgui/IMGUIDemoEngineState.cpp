//
//  IMGUIDemoEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_DESKTOP

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void IMGUIDemoEngineState::onEnter(Engine* e)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplGlfw_InitForOpenGL(INST_REG.get<GLFWwindow>("GLFWwindow"), true);
    ImGui_ImplOpenGL3_Init("#version 110");
}

void IMGUIDemoEngineState::onAssetsLoaded(Engine *e)
{
    // Empty
}

void IMGUIDemoEngineState::onExit(Engine* e)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void IMGUIDemoEngineState::onUpdate(Engine* e)
{
    uint8_t inputState = _inputProcessor.update();
    switch (inputState)
    {
        case IPS_EXIT:
            e->popState();
            return;
    }
}

void IMGUIDemoEngineState::onRender(Renderer& r)
{
    r.bindFramebuffer("main");
    // TODO, render game world
    r.renderFramebufferToScreen("main");
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // TODO, show IMGUI windows
    ImGui::ShowDemoWindow();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

IMGUIDemoEngineState::IMGUIDemoEngineState() : EngineState("json/game/Config.json"),
_inputProcessor()
{
    // Empty
}

#endif /* IS_DESKTOP */
