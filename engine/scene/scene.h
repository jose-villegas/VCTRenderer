#pragma once

#include "node.h"
#include "../util/single_active.h"

class Camera;
class MeshDrawer;
class Texture2D;
class Material;
class Light;
class Node;

class Scene : public BaseObject, public SingleActive<Scene>
{
    protected:
        std::string filepath;
        std::string directory;

        bool isLoaded;
        bool isImported;
    public:
        std::vector<std::shared_ptr<MeshDrawer>> meshes;
        std::vector<std::shared_ptr<Texture2D>> textures;
        std::vector<std::shared_ptr<Material>> materials;

        std::vector<std::shared_ptr<Camera>> cameras;
        std::vector<std::shared_ptr<Light>> lights;

        Node rootNode;

        explicit Scene(std::string filepath);
        ~Scene();

        const std::string &GetFilepath() const
        {
            return filepath;
        }

        const std::string &GetDirectory() const
        {
            return directory;
        }

        void Import();
        void Load();
        bool IsLoaded() const { return isLoaded; }
    private:
        // friends with
        friend class SceneImporter;
};

