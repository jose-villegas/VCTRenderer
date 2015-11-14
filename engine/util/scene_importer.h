#pragma once
#include "..\scene\scene.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"

#include "texture_importer.h"

class SceneImporter
{
    private:
        TextureImporter textureImporter;
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

inline const std::string GetExtension(const std::string &sFilepath)
{
    std::string result = sFilepath.substr(sFilepath.find_last_of(".") + 1);
    return result == sFilepath ? "" : result;
}

