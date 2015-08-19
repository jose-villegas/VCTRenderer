#include "stdafx.h"
#include "vct_interface.h"
#include "imgui\imgui.h"


void VCTInterface::Draw()
{
    NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    //ImGui::ShowTestWindow();
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
    {
        ImGui::SetNextWindowPos(ImVec2(3, 3));
        ImGui::Begin("Scene", nullptr,
                     ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoMove);
        ImGui::Combo("", &activeScene, (const char**)availableScenes.data(),
                     availableScenes.size());
        ImGui::End();
    }
}

VCTInterface::VCTInterface()
{
    const char* items[] = {"Sponza", "Cathedral"};
    int availableScenesSize = ((int)(sizeof(items) / sizeof(*items)));
    activeScene = -1;

    for(int i = 0; i < availableScenesSize; i++)
    {
        this->availableScenes.push_back((char *)items[i]);
    }
}


VCTInterface::~VCTInterface()
{
}
