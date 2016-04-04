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
#include "../assets/code/programs/depth_program.h"
#include "../assets/code/programs/radiance_program.h"
#include "../assets/code/programs/blur_program.h"
#include "../assets/code/programs/mipmapping_program.h"
// include interfaces
#include "../assets/code/interfaces/scene_loader.h"
#include "../assets/code/interfaces/framerate.h"
#include "../assets/code/interfaces/main_menu.h"
#include "../assets/code/interfaces/scene_cameras.h"
#include "../assets/code/interfaces/scene_lights.h"
#include "../assets/code/interfaces/framebuffer_textures.h"
#include "../assets/code/interfaces/shadowing_options.h"
#include "../assets/code/interfaces/voxelization_options.h"
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
    scenes["Crytek Sponza"] = std::make_shared<Scene>
                              ("assets\\models\\crytek-sponza\\sponza.obj");
    scenes["Crytek Sponza (No Flag)"] = std::make_shared<Scene>
                                        ("assets\\models\\crytek-sponza-noflag\\sponza.obj");
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
    interfaces["Framebuffers"] = std::make_shared<UIFramebuffers>();
    interfaces["Shadowing"] = std::make_shared<UIShadowingOptions>();
    interfaces["Voxelization"] = std::make_shared<UIVoxelizationOptions>();
    // instantiate implemented behaviors
    {
    };
    // instantiate implemented programs
    programs["Geometry"] = std::make_shared<GeometryProgram>();
    programs["Lighting"] = std::make_shared<LightingProgram>();
    programs["Voxelization"] = std::make_shared<VoxelizationProgram>();
    programs["VoxelDrawer"] = std::make_shared<VoxelDrawerProgram>();
    programs["Depth"] = std::make_shared<DepthProgram>();
    programs["InjectRadiance"] = std::make_shared<InjectRadianceProgram>();
    programs["MipmappingBase"] = std::make_shared<MipmappingBaseProgram>();
    programs["MipmappingVolume"] = std::make_shared<MipmappingVolumeProgram>();
    programs["Blur"] = std::make_shared<BlurProgram>();
    // instantiate impleted renderers
    renderers["Shadowmapping"] = std::make_shared<ShadowMapRenderer>(window);
    renderers["Voxelizer"]     = std::make_shared<VoxelizerRenderer>(window);
    renderers["Deferred"]      = std::make_shared<DeferredRenderer>(window);
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
    programs["Depth"]->AttachShader(oglplus::ShaderType::Vertex,
                                    "assets\\shaders\\depth_texture.vert");
    programs["Depth"]->AttachShader(oglplus::ShaderType::Fragment,
                                    "assets\\shaders\\depth_texture.frag");
    programs["InjectRadiance"]->AttachShader(oglplus::ShaderType::Compute,
            "assets\\shaders\\inject_radiance.comp");
    programs["MipmappingBase"]->AttachShader(oglplus::ShaderType::Compute,
            "assets\\shaders\\aniso_mipmapbase.comp");
    programs["MipmappingVolume"]->AttachShader(oglplus::ShaderType::Compute,
            "assets\\shaders\\aniso_mipmapvolume.comp");
    programs["Blur"]->AttachShader(oglplus::ShaderType::Vertex,
                                   "assets\\shaders\\blur.vert");
    programs["Blur"]->AttachShader(oglplus::ShaderType::Fragment,
                                   "assets\\shaders\\blur.frag");

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
