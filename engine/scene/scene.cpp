#include "scene.h"

#include "light.h"
#include "camera.h"

#include "../util/scene_importer.h"
#include "../misc/utils.h"
#include "mesh.h"
#include "texture.h"

Scene::Scene(std::string filepath)
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

void Scene::Load()
{
    if (isLoaded) { return; }

    SceneImporter::Import(filepath, this);

    for (auto &mesh : meshes)
    {
        mesh->Load();
    }

    for (auto &texture : textures)
    {
        texture->Load();
    }

    if (name.empty()) { name = rootNode.name; }

    cameras.front()->SetAsActive();
    isLoaded = true;
}
