#include "assets_manager.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../scene/texture.h"
#include "../scene/scene.h"

// include program shaders
#include "../assets/code/programs/lighting_program.h"
#include "../assets/code/programs/geometry_program.h"
#include "../assets/code/programs/voxelization_program.h"
#include "../assets/code/programs/voxel_drawer_program.h"
// include interfaces
#include "../assets/code/interfaces/scene_loader.h"
#include "../assets/code/interfaces/framerate.h"
#include "../assets/code/interfaces/main_menu.h"
#include "../assets/code/interfaces/scene_cameras.h"
#include "../assets/code/interfaces/scene_lights.h"
#include "../assets/code/interfaces/geometrybuffer_textures.h"
// include behaviors
// include renderers
#include "../assets/code/renderers/voxelizer_renderer.h"
#include "../assets/code/renderers/deferred_renderer.h"
#include "../assets/code/renderers/shadow_map_renderer.h"

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
    auto &window = EngineBase::Instance()->Window();
    // instantiate scenes with their paths
    scenes["Sponza"] = std::make_shared<Scene>
                       ("assets\\models\\crytek-sponza\\sponza.obj");
    scenes["Sibenik"] = std::make_shared<Scene>
                        ("assets\\models\\sibenik\\sibenik.obj");
    scenes["Cornellbox"] = std::make_shared<Scene>
                           ("assets\\models\\cornell-box\\cornellbox-original.obj");
    // instantiate implemented interfaces
    interfaces["SceneLoader"] = std::make_shared<UISceneLoader>();
    interfaces["Framerate"] = std::make_shared<UIFramerate>();
    interfaces["MainMenu"] = std::make_shared<UIMainMenu>();
    interfaces["Cameras"] = std::make_shared<UISceneCameras>();
    interfaces["Lights"] = std::make_shared<UISceneLights>();
    interfaces["GBuffer"] = std::make_shared<UIGeometryBuffer>();
    // instantiate implemented behaviors
    {
    };
    // instantiate implemented programs
    programs["Geometry"] = std::make_shared<GeometryProgram>();
    programs["Lighting"] = std::make_shared<LightingProgram>();
    programs["Voxelization"] = std::make_shared<VoxelizationProgram>();
    programs["VoxelDrawer"] = std::make_shared<VoxelDrawerProgram>();
    // instantiate impleted renderers
    renderers["Voxelizer"] = std::make_shared<VoxelizerRenderer>(window);
    renderers["Shadowmapping"] = std::make_shared<ShadowMapRenderer>(window);
    renderers["Deferred"] = std::make_shared<DeferredRenderer>(window);
    // attach shaders, ej: programs[index]->AttachShader();
    programs["Geometry"]->AttachShader(oglplus::ShaderType::Vertex,
                                       "assets\\shaders\\geometry_pass.vert");
    programs["Geometry"]->AttachShader(oglplus::ShaderType::Fragment,
                                       "assets\\shaders\\geometry_pass.frag");
    programs["Lighting"]->AttachShader(oglplus::ShaderType::Vertex,
                                       "assets\\shaders\\light_pass.vert");
    programs["Lighting"]->AttachShader(oglplus::ShaderType::Fragment,
                                       "assets\\shaders\\light_pass.frag");
    programs["Voxelization"]->AttachShader(oglplus::ShaderType::Vertex,
                                           "assets\\shaders\\voxelization.vert");
    programs["Voxelization"]->AttachShader(oglplus::ShaderType::Geometry,
                                           "assets\\shaders\\voxelization.geom");
    programs["Voxelization"]->AttachShader(oglplus::ShaderType::Fragment,
                                           "assets\\shaders\\voxelization.frag");
    programs["VoxelDrawer"]->AttachShader(oglplus::ShaderType::Vertex,
                                          "assets\\shaders\\draw_voxels.vert");
    programs["VoxelDrawer"]->AttachShader(oglplus::ShaderType::Geometry,
                                          "assets\\shaders\\draw_voxels.geom");
    programs["VoxelDrawer"]->AttachShader(oglplus::ShaderType::Fragment,
                                          "assets\\shaders\\draw_voxels.frag");

    // link and extract uniforms from shaders
    for (auto &prog : programs)
    {
        prog.second->Link();
        prog.second->ExtractUniforms();
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
