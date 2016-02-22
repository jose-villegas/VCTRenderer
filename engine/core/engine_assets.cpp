#include "engine_assets.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../programs/lighting_program.h"
#include "../programs/geometry_program.h"
#include "../scene/texture.h"
#include "../scene/scene.h"

#include "../assets/code/ui/scene_loader.h"
#include "../assets/code/ui/framerate.h"
#include "../assets/code/ui/main_menu.h"
#include "../assets/code/ui/scene_cameras.h"
#include "../assets/code/ui/scene_lights.h"

std::unique_ptr<AssetsManager> &AssetsManager::Instance()
{
    static std::unique_ptr<AssetsManager> instance = nullptr;

    if (!instance)
    {
        instance.reset(new AssetsManager());
    }

    return instance;
}

void AssetsManager::Terminate()
{
    delete Instance().release();
}

AssetsManager::AssetsManager()
{
    // associate scene paths
    scenes =
    {
        std::make_shared<Scene>
        ("assets\\models\\crytek-sponza\\sponza.obj" ),
        std::make_shared<Scene>
        ("assets\\models\\sibenik\\sibenik.obj" ),
        std::make_shared<Scene>
        ("assets\\models\\cornell-box\\cornellbox-original.obj")
    };
    // instantiate implemented interfaces
    interfaces =
    {
        std::make_shared<UISceneLoader>(),
        std::make_shared<UIFramerate>(),
        std::make_shared<UIMainMenu>(),
        std::make_shared<UISceneCameras>(),
        std::make_shared<UISceneLights>(),
    };
    // load shaders sources
    programs =
    {
        std::make_shared<GeometryProgram>
        ("assets\\shaders\\geometry_pass.vert",
        "assets\\shaders\\geometry_pass.frag"),
        std::make_shared<LightingProgram>
        ("assets\\shaders\\light_pass.vert",
        "assets\\shaders\\light_pass.frag")
    };
    Texture2D::GetDefaultTexture();
}


AssetsManager::~AssetsManager()
{
    // early owner-ship release for static scope (no gl context)
    auto dTexture = Texture2D::GetDefaultTexture().release();

    if (dTexture != nullptr) { delete dTexture; }
}
