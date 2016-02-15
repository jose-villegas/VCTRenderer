#include "engine_assets.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../programs/lighting_program.h"
#include "../programs/geometry_program.h"
#include "../scene/texture.h"
#include "../scene/scene.h"

#include "../assets/code/ui/scene_loader.h"

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
    scenes.push_back
    (
        std::make_shared<Scene>("assets\\models\\crytek-sponza\\sponza.obj")
    );
    scenes.push_back
    (
        std::make_shared<Scene>("assets\\models\\sibenik\\sibenik.obj")
    );
    scenes.push_back
    (
        std::make_shared<Scene>("assets\\models\\cornell-box\\cornellbox-original.obj")
    );
    // instantiate implemented interfaces
    interfaces.push_back
    (
        std::make_shared<UISceneLoader>()
    );
    // load shaders sources
    programs.push_back
    (
        std::make_shared<GeometryProgram>("assets\\shaders\\geometry_pass.vert",
                                          "assets\\shaders\\geometry_pass.frag")
    );
    programs.push_back
    (
        std::make_shared<LightingProgram>("assets\\shaders\\light_pass.vert",
                                          "assets\\shaders\\light_pass.frag")
    );
    OGLTexture2D::GetDefaultTexture();
}


AssetsManager::~AssetsManager()
{
    // early owner-ship release for static scope (no gl context)
    auto dTexture = OGLTexture2D::GetDefaultTexture().release();

    if (dTexture != nullptr) { delete dTexture; }
}
