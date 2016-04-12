#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shapes.h"

#include <assimp/postprocess.h>
#include "../../scene/scene.h"
#include "../../util/scene_importer.h"

std::unordered_map<std::string, std::shared_ptr<Node>> Shapes::shapes;

void Shapes::Load()
{
    if (shapes.size() > 0)
    {
        return;
    }

    Scene primitiveScene("rendering\\primitives\\primitives.obj");
    SceneImporter::Import("rendering\\primitives\\primitives.obj", &primitiveScene,
                          aiProcessPreset_TargetRealtime_MaxQuality);

    for (auto mesh : primitiveScene.meshes)
    {
        mesh->Load();
    }

    for (auto &node : primitiveScene.rootNode->nodes)
    {
        shapes[node->name] = move(node);
    }

    Transform::CleanEventMap();
}

std::vector<std::string> Shapes::ShapeNameList()
{
    if (shapes.size() == 0) Load();

    std::vector<std::string> names;

    for(auto &shape : shapes)
    {
        names.push_back(shape.first);
    }

    return names;
}

std::shared_ptr<Node> Shapes::GetShape(const std::string &name)
{
    if (shapes.size() == 0) Load();

    auto it = shapes.find(name);

    if (it != shapes.end()) return it->second;

    return nullptr;
}

void Shapes::UnloadShapes()
{
    shapes.clear();
}
