#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>

struct aiLight;
struct aiCamera;
struct aiNode;
struct aiMesh;
struct aiMaterial;
class Light;
class Camera;
class Node;
class Mesh;
class Material;
class Scene;

/// <summary>
/// Utilizes Assimp library to import scenes
/// info into the <see cref="Scene"></see> class.
/// </summary>
class SceneImporter
{
    public:
        SceneImporter();
        virtual ~SceneImporter();
        static bool Import(const std::string &filepath, Scene * scene, unsigned flags);
        // fine imports
        static void ImportMaterial(aiMaterial * mMaterial, Material &material);
        static void ImportMesh(aiMesh * mMesh, Mesh &mesh);
        static void ProcessNodes(Scene * scene, aiNode * mNode, Node &node);
        static void ImportCamera(aiCamera * mCam, Camera &camera);
        static void ImportLight(aiLight * mLight, Light &light);
        static void ImportMaterialTextures(Scene * scene, aiMaterial * mMaterial,
                                           Material &material);
};

