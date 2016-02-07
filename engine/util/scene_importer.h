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
        bool Import(const std::string &sFilepath, Scene &outScene);
        // fine imports
        void ImportMaterial(aiMaterial * mMaterial, OGLMaterial &outMaterial);
        void ImportMesh(aiMesh * mMesh, Mesh &outMesh);
        void ProcessNodes(Scene &scene, aiNode * node, Node &newNode);
        void ImportCamera(aiCamera * cam, Camera &newCamera);
        void ImportLight(aiLight * light, Light &newLight);
        void ImportMaterialTextures(Scene &scene, aiMaterial * mMaterial,
                                    OGLMaterial &material);
};

