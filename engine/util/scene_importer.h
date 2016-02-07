#pragma once

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
class OGLMaterial;
class Scene;

class SceneImporter
{
    public:
        SceneImporter();
        virtual ~SceneImporter();
        static bool Import(const std::string &sFilepath, Scene * outScene) ;
        // fine imports
        static void ImportMaterial(aiMaterial * mMaterial, OGLMaterial &outMaterial);
        static void ImportMesh(aiMesh * mMesh, Mesh &outMesh);
        static void ProcessNodes(Scene * scene, aiNode * node, Node &newNode);
        static void ImportCamera(aiCamera * cam, Camera &newCamera);
        static void ImportLight(aiLight * light, Light &newLight);
        static void ImportMaterialTextures(Scene * scene, aiMaterial * mMaterial,
                                           OGLMaterial &material);
};

