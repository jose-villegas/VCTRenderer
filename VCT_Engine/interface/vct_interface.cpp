#include "stdafx.h"
#include "vct_interface.h"
#include "imgui\imgui.h"
#include "..\util\scene_importer.h"
#include "core\engine_base.h"

void UI::Draw()
{
    // information resources for gui building
    ImGuiIO&  io = ImGui::GetIO();
    static EngineAssets * engineAssets = &EngineBase::Instance()->GetAssets();
    static ExecutionInfo * executionInfo = &EngineBase::Instance()->GetExecInfo();
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
        ImGui::Combo("Active", &executionInfo->activeScene,
                     (const char**)engineAssets->GetAvailableScenes().data(),
                     (int)engineAssets->GetAvailableScenes().size());
        ImGui::PopItemWidth();
        //ImGui::ShowTestWindow();
        ImGui::End();
    }
}

UI::UI()
{
}


UI::~UI()
{
}
