#include "scene.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "mesh.h"
#include "light.h"
#include "camera.h"
#include "texture.h"
#include "../util/scene_importer.h"
#include "../misc/utils.h"

Scene::Scene(std::string filepath): isLoaded(false), isImported(false)
{
    this->filepath = filepath;
    this->directory = Utils::GetDirectoryPath(filepath);
    // scenes have by default at least one camera
    // they will be removed if camera or light info
    // is found during scene extraction
    this->cameras.push_back(std::make_shared<Camera>());
    this->lights.push_back(std::make_shared<Light>());
}

Scene::~Scene()
{
}

void Scene::Import()
{
    if (isImported || isLoaded) { return; }

    SceneImporter::Import(filepath, this);
    isImported = true;
}

void Scene::Load()
{
    if (isLoaded || !isImported) { return; }

    for (auto &mesh : meshes)
    {
        mesh->Load();
    }

    for (auto &texture : textures)
    {
        texture->Load
        (
            oglplus::TextureMinFilter::LinearMipmapLinear,
            oglplus::TextureMagFilter::Linear,
            oglplus::TextureWrap::Repeat,
            oglplus::TextureWrap::Repeat
        );
    }

    if (name.empty()) { name = rootNode.name; }

    cameras.front()->SetAsActive();
    isLoaded = true;
}
