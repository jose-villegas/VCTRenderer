#include "stdafx.h"
#include "vct_interface.h"
#include "imgui\imgui.h"
#include "..\util\scene_importer.h"
#include "core\base.h"
using namespace VCT_ENGINE;

void UI::Draw()
{
    NewFrame();
    static ImGuiIO& io = ImGui::GetIO();
    ExecutionInfo * execInfo = &Base::Instance()->execInfo;
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
        // active scene selector
        ImGui::PushItemWidth(350);
        ImGui::Combo("Active", &execInfo->activeScene,
                     (const char**)execInfo->availableScenes.data(),
                     (int)execInfo->availableScenes.size());
        ImGui::PopItemWidth();
        //ImGui::ShowTestWindow();
        ImGui::End();
    }
}

UI::UI()
{
    //activeScene = -1;
}


UI::~UI()
{
}
