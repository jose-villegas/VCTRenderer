#include "assets_manager.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../scene/texture.h"
#include "../types//program_shader.h"

// include program shaders
// include interfaces
// include behaviors
// include renderers

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
    // instantiate scenes with their paths
    scenes =
    {
    };
    // instantiate implemented interfaces
    interfaces =
    {
    };
    // instantiate implemented behaviors
    behaviors =
    {
    };
    // instantiate implemented programs
    programs =
    {
    };
    // instantiate impleted renderers
    renderers =
    {
    };
    // attach shaders, ej: programs[index]->AttachShader();

    // link and extract uniforms from shaders
    for (auto &prog : programs)
    {
        prog->Link();
        prog->ExtractUniforms();
    }

    // utility default assets
    Texture2D::GetDefaultTexture();
}


AssetsManager::~AssetsManager()
{
    // early owner-ship release for static scope (no gl context)
    auto dTexture = Texture2D::GetDefaultTexture().release();

    if (dTexture != nullptr) { delete dTexture; }
}
