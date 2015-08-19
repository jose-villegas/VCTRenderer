#include "stdafx.h"
#include "interface.h"
#include "imgui\imgui_impl_glfw_gl3.h"
#include "imgui\imgui.h"

Interface::Interface()
{
}


Interface::~Interface()
{
}

void Interface::Initialize(const RenderWindow &activeWindow)
{
    ImGui_ImplGlfwGL3_Init(activeWindow.Handler(), true);
}

void Interface::Terminate()
{
    ImGui_ImplGlfwGL3_Shutdown();
}

void Interface::Render()
{
    ImGui::Render();
}

void Interface::Draw()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfwGL3_NewFrame();
    // performance window
    {
        ImGui::SetNextWindowSize(ImVec2(150, 50));
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 150 - 3,
                                       io.DisplaySize.y - 50 - 3));
        ImGui::Begin("Performance Window", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Performance");
        ImGui::Separator();
        ImGui::Text("FPS: (%.1f)", io.Framerate);
        ImGui::End();
    }
}
