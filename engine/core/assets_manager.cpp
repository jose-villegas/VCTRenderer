#include "assets_manager.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../scene/texture.h"
#include "../scene/scene.h"

#include "../programs/lighting_program.h"
#include "../programs/geometry_program.h"

#include "../assets/code/interfaces/scene_loader.h"
#include "../assets/code/interfaces/framerate.h"
#include "../assets/code/interfaces/main_menu.h"
#include "../assets/code/interfaces/scene_cameras.h"
#include "../assets/code/interfaces/scene_lights.h"
#include "../assets/code/interfaces/geometrybuffer_textures.h"

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
        std::make_shared<UIGeometryBuffer>(),
    };
    // resize programs holder
    programs.resize(CorePrograms::Size);
    // load shaders sources
    programs =
    {
        std::make_shared<GeometryProgram>(),
        std::make_shared<LightingProgram>()
    };
    // attach shader source paths
    programs[0]->AttachShader(oglplus::ShaderType::Vertex,
                              "assets\\shaders\\geometry_pass.vert");
    programs[0]->AttachShader(oglplus::ShaderType::Fragment,
                              "assets\\shaders\\geometry_pass.frag");
    programs[1]->AttachShader(oglplus::ShaderType::Vertex,
                              "assets\\shaders\\light_pass.vert");
    programs[1]->AttachShader(oglplus::ShaderType::Fragment,
                              "assets\\shaders\\light_pass.frag");
    // utility default assets
    Texture2D::GetDefaultTexture();
}


AssetsManager::~AssetsManager()
{
    // early owner-ship release for static scope (no gl context)
    auto dTexture = Texture2D::GetDefaultTexture().release();

    if (dTexture != nullptr) { delete dTexture; }
}
