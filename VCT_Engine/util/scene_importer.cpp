#include "stdafx.h"
#include "scene_importer.h"

SceneImporter::SceneImporter()
{
}


SceneImporter::~SceneImporter()
{
}

bool SceneImporter::Import(const std::string &sFilepath, Scene &outScene)
{
    Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile(sFilepath, aiProcess_GenSmoothNormals
                            | aiProcess_CalcTangentSpace
                            | aiProcess_Triangulate
                            | aiProcess_JoinIdenticalVertices
                            | aiProcess_SortByPType);

    if(!scene)
    {
        std::cout << "Error loading file (Assimp): " << importer.GetErrorString() <<
                  std::endl;
        return false;
    }

    // load materials

    if(scene->HasMaterials())
    {
        for(unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            outScene.materials.push_back(std::move(ImportMaterial(scene->mMaterials[i])));
        }
    }
}

Material SceneImporter::ImportMaterial(aiMaterial* mMaterial)
{
    Material newMaterial;
    // assimp scene material name extract
    aiString materialName;
    mMaterial->Get(AI_MATKEY_NAME, materialName);

    if(materialName.length > 0)
    {
        newMaterial.name = materialName.C_Str();
    }

    // material factors
    mMaterial->Get(AI_MATKEY_SHADING_MODEL, newMaterial.shadingModel);
    mMaterial->Get(AI_MATKEY_BLEND_FUNC, newMaterial.blendMode);
    mMaterial->Get(AI_MATKEY_REFRACTI, newMaterial.refractionIndex);
    mMaterial->Get(AI_MATKEY_SHININESS, newMaterial.shininess);
    mMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, newMaterial.shininessStrenght);
    // get material properties
    aiColor3D ambient(0.f), diffuse(0.f), specular(0.f);
    aiColor3D emissive(0.f), transparent(0.f);
    mMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    mMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    mMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    mMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
    mMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent);
    newMaterial.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
    newMaterial.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
    newMaterial.specular = glm::vec3(specular.r, specular.g, specular.b);
    newMaterial.emissive = glm::vec3(emissive.r, emissive.g, emissive.b);
    newMaterial.transparent = glm::vec3(transparent.r, transparent.g,
                                        transparent.b);
    return newMaterial;
}
