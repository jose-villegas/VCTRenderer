#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "framerate.h"

#include "main_menu.h"

using namespace ImGui;

void UIFramerate::Draw()
{
    if (!UIMainMenu::drawFramerate) { return; }

    static auto openFPSWindow = false;
    static auto position = ImVec2(io->DisplaySize.x - 80 - 3,
                                  io->DisplaySize.y - 50 - 3);
    SetNextWindowPos(position);

    if (Begin("Performance Window", nullptr, ImGuiWindowFlags_NoTitleBar |
              ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        Text("Framerate");
        Separator();
        Text("%.3f", io->Framerate);
    }

    position = GetWindowPos();
    End();
}

UIFramerate::UIFramerate()
{
}

UIFramerate::~UIFramerate()
{
}
