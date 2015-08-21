#include "stdafx.h"
#include "scene_importer.h"
#include "..\scene\scene.h"

SceneImporter::SceneImporter()
{
}


SceneImporter::~SceneImporter()
{
}

bool SceneImporter::Import(const std::string &sFilepath, Scene &outScene)
{
    Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile(sFilepath,
                            aiProcessPreset_TargetRealtime_Fast);

    if(!scene)
    {
        std::cout << "Error loading file (Assimp): " << importer.GetErrorString() <<
                  std::endl;
        return false;
    }

    if(scene->HasMaterials())
    {
        for(unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            outScene.materials.push_back(std::move(ImportMaterial(scene->mMaterials[i])));
        }
    }

    if(scene->HasMeshes())
    {
        for(unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            outScene.meshes.push_back(std::move(ImportMesh(scene->mMeshes[i])));
        }
    }
}

Material SceneImporter::ImportMaterial(aiMaterial *mMaterial)
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

    for(unsigned int i = aiTextureType::aiTextureType_NONE;
        i < aiTextureType::aiTextureType_UNKNOWN; i++)
    {
        int textureTypeCount = mMaterial->GetTextureCount((aiTextureType)i);

        for(int typeIndex = 0; typeIndex < textureTypeCount; typeIndex++)
        {
            aiString textureFilepath;

            if(mMaterial->GetTexture((aiTextureType)i, typeIndex,
                                     &textureFilepath) == AI_SUCCESS)
            {
                std::string fullPath = textureFilepath.data;
                std::cout << fullPath << std::endl;
            }
        }
    }

    return newMaterial;
}

Mesh SceneImporter::ImportMesh(aiMesh *mMesh)
{
    Mesh newMesh;
    newMesh.name = mMesh->mName.length > 0 ? mMesh->mName.C_Str() : newMesh.name;

    if(mMesh->mNumVertices > 0)
    {
        for(unsigned int i = 0; i < mMesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertex.position = glm::vec3(mMesh->mVertices[i].x,
                                        mMesh->mVertices[i].y,
                                        mMesh->mVertices[i].z);
            vertex.normal = glm::vec3(mMesh->mNormals[i].x,
                                      mMesh->mNormals[i].y,
                                      mMesh->mNormals[i].z);
            vertex.uv = glm::vec2(mMesh->mTextureCoords[0]->x,
                                  mMesh->mTextureCoords[0]->y);

            if(mMesh->HasTangentsAndBitangents())
            {
                vertex.tangent = glm::vec3(mMesh->mTangents[i].x,
                                           mMesh->mTangents[i].y,
                                           mMesh->mTangents[i].z);
                vertex.bitangent = glm::vec3(mMesh->mBitangents[i].x,
                                             mMesh->mBitangents[i].y,
                                             mMesh->mBitangents[i].z);
            }

            vertex.Orthonormalize();
            newMesh.vertices.push_back(std::move(vertex));
        }
    }

    for(unsigned int i = 0; i < mMesh->mNumFaces; i++)
    {
        newMesh.indices.push_back(mMesh->mFaces[i].mIndices[0]);
        newMesh.indices.push_back(mMesh->mFaces[i].mIndices[1]);
        newMesh.indices.push_back(mMesh->mFaces[i].mIndices[2]);
    }

    return newMesh;
}
