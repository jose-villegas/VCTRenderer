#include "engine_assets.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../scene/texture.h"
#include "../scene/scene.h"
#include "../interface/vct_interface.h"

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
    scenes.push_back
    (
        std::make_shared<Scene>("resources\\models\\crytek-sponza\\sponza.obj")
    );
    scenes.push_back
    (
        std::make_shared<Scene>("resources\\models\\sibenik\\sibenik.obj")
    );
    scenes.push_back
    (
        std::make_shared<Scene>("resources\\models\\cornell-box\\cornellbox-original.obj")
    );
    interfaces.push_back
    (
        std::make_shared<UI>()
    );
    OGLTexture2D::GetDefaultTexture();
}


AssetsManager::~AssetsManager()
{
    scenes.clear();
    interfaces.clear();
    // early owner-ship release for static scope (no gl context)
    auto dTexture = OGLTexture2D::GetDefaultTexture().release();

    if (dTexture != nullptr) { delete dTexture; }
}
