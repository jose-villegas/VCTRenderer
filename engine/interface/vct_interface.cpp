#include "vct_interface.h"
#include <oglplus/object/name_tpl.hpp>
#include <vector>
#include <numeric>

#include "../core/deferred_handler.h"
#include "../core/engine_base.h"
#include "../core/deferred_renderer.h"
#include "../core/engine_assets.h"
#include "../scene/scene.h"
#include "../scene/camera.h"
#include <iostream>

void UI::DrawGBufferTexture(const oglplus::Texture &texture,
                            const std::string &name) const
{
    ImGui::BeginGroup();
    ImGui::Image(reinterpret_cast<void *>
                 (static_cast<intptr_t>(oglplus::GetName(texture))),
                 ImVec2(140, 140), ImVec2(1, 1), ImVec2(0, 0));

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Image(reinterpret_cast<void *>
                     (static_cast<intptr_t>(oglplus::GetName(texture))),
                     ImVec2(io.DisplaySize.x / 2.0f, io.DisplaySize.y / 2.0f),
                     ImVec2(1, 1), ImVec2(0, 0));
        ImGui::EndTooltip();
    }

    ImGui::Text(name.c_str());
    ImGui::EndGroup();
}

void UI::DrawFramerateMetrics() const
{
    // frame rate samples for plotline
    static auto minfSample = std::numeric_limits<float>::infinity();
    static auto maxfSample = std::numeric_limits<float>::lowest();
    static auto numSamples = 100;
    static std::vector<float> frameSamples(numSamples);
    auto accum = accumulate(frameSamples.begin(), frameSamples.end(), 0.0f);
    // start window
    ImGui::Text("Framerate: (%.1f)", io.Framerate);
    // average framerate
    ImGui::SameLine();
    ImGui::Text("Avg/Min/Max: (%.1f, %.1f, %.1f)", accum / frameSamples.size(),
                minfSample, maxfSample);
    ImGui::SameLine();

    if (ImGui::Button("Reset"))
    {
        minfSample = std::numeric_limits<float>::infinity();
        maxfSample = std::numeric_limits<float>::lowest();
    }

    // num of framerate samples

    if (ImGui::InputInt("# Frame Samples", &numSamples))
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
    if (frameSamples.size() > static_cast<size_t>(numSamples))
    {
        frameSamples.erase(frameSamples.begin());
    }

    // draw plotline with frame samples
    ImGui::PlotLines("##Lines", frameSamples.data(),
                     static_cast<int>(frameSamples.size()), 0,
                     "", minfSample, maxfSample, ImVec2(584.0f, 80.0f));
}

bool SceneName(void * data, int idx, const char ** out_text)
{
    auto items = static_cast<std::shared_ptr<Scene> *>(data);

    if (out_text)
    {
        *out_text = items[idx]->GetFilepath().c_str();
    }

    return true;
}

void UI::DrawSceneSelector()
{
    static auto &assets = EngineBase::Assets();
    static auto activeScene = -1;
    ImGui::SetNextWindowPos(ImVec2(3, 3));
    ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize |
                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);
    // active scene selector
    ImGui::PushItemWidth(450);

    if (ImGui::Combo("Path", &activeScene, SceneName, assets.scenes.data(),
                     static_cast<int>(assets.scenes.size())))
    {
        assets.scenes[activeScene]->SetAsActive();
        assets.scenes[activeScene]->cameras.front()->SetAsActive();
    }

    ImGui::SameLine();

    if (ImGui::Button("Load") && Scene::Active())
    {
        Scene::Active()->Load();
    }

    ImGui::PopItemWidth();
    uiPos = ImGui::GetWindowPos();
    uiSize = ImGui::GetWindowSize();
    ImGui::End();
}

void UI::DrawGBufferTextures() const
{
    static auto &gbuffer = EngineBase::Renderer().GeometryBuffer();
    // position texture
    DrawGBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Position),
                       "Position");
    ImGui::SameLine();
    // normal texture
    DrawGBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Normal),
                       "Normal");
    ImGui::SameLine();
    // albedo texture
    DrawGBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Albedo),
                       "Albedo");
    ImGui::SameLine();
    // specular texture
    DrawGBufferTexture(gbuffer.RenderTarget(GeometryBuffer::Specular),
                       "Specular");
}

void UI::DrawDebugWindow() const
{
    static bool openDebugWindow;
    openDebugWindow ^= io.KeyCtrl && ImGui::IsKeyReleased(74);

    if (openDebugWindow) // debug window
    {
        ImGui::SetNextWindowPos(
            ImVec2(uiPos.x, uiPos.y + uiSize.y)
        );
        ImGui::Begin("Debug Info", &openDebugWindow,
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoMove);

        if (ImGui::CollapsingHeader("Framerate Metrics"))
        {
            DrawFramerateMetrics();
        }

        if (ImGui::CollapsingHeader("Geometry Buffer Textures"))
        {
            DrawGBufferTextures();
        }

        ImGui::End();
    }
}

void UI::DrawFPSNotif() const
{
    static auto openFPSWindow = false;
    openFPSWindow ^= io.KeyCtrl && ImGui::IsKeyReleased(70);

    if (openFPSWindow)
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

void UI::Draw()
{
    // scene selector
    DrawSceneSelector();
    DrawDebugWindow();
    // show only fps
    DrawFPSNotif();
    // test movement
    auto &camera = Camera::Active();
    static float speed = 5.0f;
    float cameraSpeed = speed * io.DeltaTime;

    if (!camera) { return; }

    static float yaw, pitch;
    float sensitivity = 0.135f;
    float xOffset = io.MouseDelta.x;
    float yOffset = -io.MouseDelta.y;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    yaw = yaw + xOffset;
    pitch = pitch + yOffset;
    //if (pitch > 89.0f)
    //{
    //    pitch = 89.0f;
    //}
    //if (pitch < -89.0f)
    //{
    //    pitch = -89.0f;
    //}
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = normalize(front);
    auto right = normalize(cross(front, camera->Up()));

    if (ImGui::IsKeyDown(GLFW_KEY_W))
    {
        camera->Position(camera->Position() + front * cameraSpeed);
    }

    if (ImGui::IsKeyDown(GLFW_KEY_S))
    {
        camera->Position(camera->Position() - front * cameraSpeed);
    }

    if (ImGui::IsKeyDown(GLFW_KEY_D))
    {
        camera->Position(camera->Position() + right * cameraSpeed);
    }

    if (ImGui::IsKeyDown(GLFW_KEY_A))
    {
        camera->Position(camera->Position() - right * cameraSpeed);
    }

    if (ImGui::IsKeyDown(GLFW_KEY_Q))
    {
        camera->Position(camera->Position() + camera->Up() * cameraSpeed);
    }

    if (ImGui::IsKeyDown(GLFW_KEY_E))
    {
        camera->Position(camera->Position() - camera->Up() * cameraSpeed);
    }

    camera->LookAt(camera->Position() + front);

    if (ImGui::IsKeyDown(GLFW_KEY_Z))
    {
        speed -= 0.75f;
    }

    if (ImGui::IsKeyDown(GLFW_KEY_X))
    {
        speed += 0.75f;
    }
}

UI::UI() : io(ImGui::GetIO()) {}
UI::~UI() {}