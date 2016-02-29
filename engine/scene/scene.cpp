#include "scene.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "mesh.h"
#include "light.h"
#include "camera.h"
#include "texture.h"
#include "../util/scene_importer.h"

void Scene::SetAsActive()
{
    // previous active scene
    auto previous = Active().get();

    if (previous != nullptr)
    {
        // save current scene active camera index
        previous->activeCamera = -1;

        for (size_t i = 0; i < previous->cameras.size(); i++)
        {
            if (previous->cameras[i]->IsActive())
            {
                previous->activeCamera = static_cast<int>(i);
            }
        }
    }

    // call base method, now this instace is marked as active
    SingleActive::SetAsActive();
    // clean current light per type collections
    Light::CleanCollections();

    // add all the current scene lights to type collection
    for (auto &light : lights)
    {
        light->TypeCollection(light->Type(), true);
    }

    // once changing scene previous active cameras
    // from other scene shouldn't be active
    if (activeCamera != -1 && activeCamera < cameras.size())
    {
        cameras[activeCamera]->SetAsActive();
    }
    else
    {
        Camera::ResetActive();
    }
}

inline std::string GetDirectoryPath(const std::string &filePath)
{
    auto pos = filePath.find_last_of("\\/");
    return (std::string::npos == pos) ? "" : filePath.substr(0, pos);
}


Scene::Scene(std::string filepath): isLoaded(false), isImported(false),
    activeCamera(0)
{
    this->filepath = filepath;
    this->directory = GetDirectoryPath(filepath);
    // scenes have by default at least one camera
    // they will be removed if camera or light info
    // is found during scene extraction
    this->cameras.push_back(std::make_shared<Camera>());
    this->lights.push_back(std::make_shared<Light>());
    // default camera created always as active
    this->cameras.back()->SetAsActive();
    // create scene root node
    this->rootNode = std::make_shared<Node>();
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

    if (name.empty()) { name = rootNode->name; }

    cameras.front()->SetAsActive();
    isLoaded = true;
}
