#pragma once

#include "node.h"
#include "../util/single_active.h"

class Camera;
class MeshDrawer;
class Texture2D;
class Material;
class Light;
class Node;

/// <summary>
/// Represents a scene composed of many cameras, meshes, lights,
/// textures and materials. Handles importing, loading and changing
/// scenes.
/// </summary>
/// <seealso cref="BaseObject" />
/// <seealso cref="SingleActive{Scene}" />
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

        std::shared_ptr<Node> rootNode;

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
        /// <summary>
        /// Imports the raw data from the specified scene at
        /// <see cref="filepath" /> into usable classes.
        /// </summary>
        void Import();
        /// <summary>
        /// Uploads to GPU the mesh and texture data
        /// </summary>
        void Load();

        /// <summary>
        /// Returns if this scene is already loaded.
        /// </summary>
        /// <returns></returns>
        bool IsLoaded() const { return isLoaded; }

        /// <summary>
        /// Sets this scene as active and executes all neccesary
        /// logic for changing the current active scene
        /// </summary>
        void SetAsActive() override;
    private:
        int activeCamera;
        // friends with
        friend class SceneImporter;
};

