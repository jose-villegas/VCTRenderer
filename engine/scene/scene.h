#pragma once

#include "node.h"
#include "../util/single_active.h"
#include "../types/instance_pool.h"

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
    public:
        std::vector<std::shared_ptr<MeshDrawer>> meshes;
        std::vector<std::shared_ptr<Texture2D>> textures;
        std::vector<std::shared_ptr<Material>> materials;
        std::vector<std::shared_ptr<Camera>> cameras;
        std::vector<std::shared_ptr<Light>> lights;
        std::shared_ptr<Node> rootNode;

        /// <summary>
        /// Gets the load scene filepath.
        /// </summary>
        /// <returns></returns>
        const std::string &GetFilepath() const;
        /// <summary>
        /// Gets the loaded scene file directory.
        /// </summary>
        /// <returns></returns>
        const std::string &GetDirectory() const;
        /// <summary>
        /// Imports the raw data from the specified scene at
        /// <see cref="filepath" /> into usable classes.
        /// </summary>
        void Import(unsigned int flags);
        /// <summary>
        /// Reimports the scene with the specified flags.
        /// This will clear any previous data.
        /// </summary>
        /// <param name="flags">The flags.</param>
        void CleanImport(unsigned int flags);
        /// <summary>
        /// Uploads to GPU the mesh and texture data
        /// </summary>
        void Load();
        /// <summary>
        /// Returns if this scene file is already loaded.
        /// </summary>
        /// <returns></returns>
        bool IsLoaded() const;
        /// <summary>
        /// Sets this scene as active and executes all neccesary
        /// logic for changing the current active scene
        /// </summary>
        void SetAsActive() override;

        explicit Scene(std::string filepath);
        ~Scene();
    protected:
        std::string filepath;
        std::string directory;

        bool isLoaded;
        bool isImported;
    private:
        int activeCamera;

        friend class SceneImporter;
};

