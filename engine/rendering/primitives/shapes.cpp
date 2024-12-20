#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shapes.h"

#include <assimp/postprocess.h>
#include "../../scene/scene.h"
#include "../../util/scene_importer.h"
#include "../../scene/texture.h"

std::unordered_map<std::string, std::shared_ptr<Node>> Shapes::shapes;

void Shapes::Load()
{
    if (shapes.size() > 0)
    {
        return;
    }

    Scene primitiveScene("rendering\\primitives\\assets\\primitives.fbx");
    SceneImporter::Import("rendering\\primitives\\assets\\primitives.fbx",
                          &primitiveScene,
                          aiProcessPreset_TargetRealtime_Fast);

    for (auto &mesh : primitiveScene.meshes)
    {
        mesh->Load();
    }

    for (auto &texture : primitiveScene.textures)
    {
        texture->Load
        (
            oglplus::TextureMinFilter::LinearMipmapLinear,
            oglplus::TextureMagFilter::Linear,
            oglplus::TextureWrap::Repeat,
            oglplus::TextureWrap::Repeat
        );
    }

    for (auto &node : primitiveScene.rootNode->nodes)
    {
        if (node->meshes.size() == 0) { continue; }

        node->BuildDrawList();
        node->nodeState = Node::Dynamic;
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

    if (it != shapes.end())
    {
        auto copyNode = std::make_shared<Node>(*it->second);
        return copyNode;
    }

    return nullptr;
}

void Shapes::UnloadShapes()
{
    shapes.clear();
}
