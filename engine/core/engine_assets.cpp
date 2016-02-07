#include "engine_assets.h"
#include "../scene/texture.h"
#include "../scene/scene.h"

EngineAssets::EngineAssets()
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
    OGLTexture2D::GetDefaultTexture();
}


EngineAssets::~EngineAssets()
{
    // early owner-ship release for static scope (no gl context)
    auto dTexture = OGLTexture2D::GetDefaultTexture().release();

    if (dTexture != nullptr) { delete dTexture; }
}
