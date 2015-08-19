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

void Interface::NewFrame()
{
    ImGui_ImplGlfwGL3_NewFrame();
}
