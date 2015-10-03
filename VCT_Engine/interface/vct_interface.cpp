#include "stdafx.h"
#include "vct_interface.h"
#include "imgui\imgui.h"
#include "..\util\scene_importer.h"
#include "core\engine_base.h"
#include "core\deferred_handler.h"


void UI::Draw()
{
    // information resources for gui building
    ImGuiIO&  io = ImGui::GetIO();
    static EngineAssets * engineAssets = &EngineBase::Instance()->GetAssets();
    static ExecutionInfo * executionInfo = &EngineBase::Instance()->GetExecInfo();
    static DeferredHandler * deferredHandler =
        &EngineBase::Instance()->GetRenderer().GetDeferredHandler();
    static ImVec2 sceneSelectorPos, sceneSelectorSize;
    // scene selector
    {
        ImGui::SetNextWindowPos(ImVec2(3, 3));
        ImGui::Begin("Scene", nullptr,
                     ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoMove);
        // active scene selector
        ImGui::PushItemWidth(450);
        ImGui::Combo("Active", &executionInfo->activeScene,
                     (const char**)engineAssets->GetAvailableScenes().data(),
                     (int)engineAssets->GetAvailableScenes().size());
        ImGui::PopItemWidth();
        sceneSelectorPos = ImGui::GetWindowPos();
        sceneSelectorSize = ImGui::GetWindowSize();
        ImGui::End();
    }
    static bool openDebugWindow = false;
    openDebugWindow ^= io.KeyCtrl && ImGui::IsKeyReleased(74);

    if(openDebugWindow)// debug window
    {
        ImGui::SetNextWindowPos(
            ImVec2(sceneSelectorPos.x, sceneSelectorPos.y + sceneSelectorSize.y)
        );
        ImGui::Begin("Debug Info", &openDebugWindow,
                     ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoMove);

        if(ImGui::CollapsingHeader("Framerate Metrics"))
        {
            // frame rate samples for plotline
            static float minfSample = std::numeric_limits<float>::infinity();
            static float maxfSample = std::numeric_limits<float>::lowest();
            static std::vector<float> frameSamples(100);
            static int numSamples = 100;
            // start window
            ImGui::Text("Framerate: (%.1f)", io.Framerate);
            float accum = std::accumulate(frameSamples.begin(), frameSamples.end(), 0.0f);
            // average framerate
            ImGui::SameLine();
            ImGui::Text("Avg/Min/Max: (%.1f, %.1f, %.1f)", accum / frameSamples.size(),
                        minfSample, maxfSample);
            ImGui::SameLine();

            if(ImGui::Button("Reset"))
            {
                minfSample = std::numeric_limits<float>::infinity();
                maxfSample = std::numeric_limits<float>::lowest();
            }

            // num of framerate samples

            if(ImGui::InputInt("# Frame Samples", &numSamples))
            {
                numSamples < 6 ? numSamples = 6 : 0;
                numSamples > 8000 ? numSamples = 8000 : 0;
                frameSamples.resize(numSamples);
            }

            // add new sample
            frameSamples.push_back(io.Framerate);
            minfSample > frameSamples.back() ? minfSample = frameSamples.back() : 0;
            maxfSample < frameSamples.back() ? maxfSample = frameSamples.back() : 0;

            // too many samples pop front
            if(frameSamples.size() > numSamples) frameSamples.erase(frameSamples.begin());

            // draw plotline with frame samples
            ImGui::PlotLines("##Lines", frameSamples.data(), frameSamples.size(), 0.0f,
                             "", minfSample, maxfSample, ImVec2(584.0f, 80.0f));
        }

        if(ImGui::CollapsingHeader("Geometry Buffer Textures"))
        {
            // position texture
            {
                ImGui::BeginGroup();
                ImGui::Image(
                    (void *)(intptr_t)GetName(
                        deferredHandler->GetGBufferTextures()
                        [DeferredHandler::GBufferTextureType::Position]
                    ),
                    ImVec2(140, 140), ImVec2(1, 1), ImVec2(0, 0)
                );
                ImGui::Text("Position");
                ImGui::EndGroup();
            }
            ImGui::SameLine();
            // normal texture
            {
                ImGui::BeginGroup();
                ImGui::Image(
                    (void *)(intptr_t)GetName(
                        deferredHandler->GetGBufferTextures()
                        [DeferredHandler::GBufferTextureType::Normal]
                    ),
                    ImVec2(140, 140), ImVec2(1, 1), ImVec2(0, 0)
                );
                ImGui::Text("Normal");
                ImGui::EndGroup();
            }
            ImGui::SameLine();
            // albedo texture
            {
                ImGui::BeginGroup();
                ImGui::Image(
                    (void *)(intptr_t)GetName(
                        deferredHandler->GetGBufferTextures()
                        [DeferredHandler::GBufferTextureType::Albedo]
                    ),
                    ImVec2(140, 140), ImVec2(1, 1), ImVec2(0, 0)
                );
                ImGui::Text("Albedo");
                ImGui::EndGroup();
            }
            ImGui::SameLine();
            // specular texture
            {
                ImGui::BeginGroup();
                ImGui::Image(
                    (void *)(intptr_t)GetName(
                        deferredHandler->GetGBufferTextures()
                        [DeferredHandler::GBufferTextureType::Specular]
                    ),
                    ImVec2(140, 140), ImVec2(1, 1), ImVec2(0, 0)
                );
                ImGui::Text("Specular");
                ImGui::EndGroup();
            }
        }

        ImGui::End();
    }

    // show only fps
    static bool openFPSWindow = false;
    openFPSWindow ^= io.KeyCtrl && ImGui::IsKeyReleased(70);

    if(openFPSWindow)
    {
        ImGui::SetNextWindowSize(ImVec2(80, 50));
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 80 - 3,
                                       io.DisplaySize.y - 50 - 3));
        ImGui::Begin("Performance Window", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Framerate");
        ImGui::Separator();
        ImGui::Text("%.3f", io.Framerate);
        ImGui::End();
    }
}

UI::UI()
{
}


UI::~UI()
{
}
