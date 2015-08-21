#include "stdafx.h"
#include "scene_importer.h"
#include "..\scene\scene.h"
#include "command_line.h"

SceneImporter::SceneImporter()
{
}


SceneImporter::~SceneImporter()
{
}

bool SceneImporter::Import(const std::string &sFilepath, Scene &outScene)
{
    std::cout << "(Assimp) Processing File: " << sFilepath << std::endl;
    Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile(sFilepath,
                            aiProcessPreset_TargetRealtime_Fast);

    if(!scene)
    {
        std::cout << "(Assimp) Error Loading File: " << importer.GetErrorString() <<
                  std::endl;
        return false;
    }

    std::cout << "(Assimp) Loading Scene:" << std::endl;

    if(scene->HasMaterials())
    {
        // process material properties
        for(unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            outScene.materials.push_back(std::move(ImportMaterial(scene->mMaterials[i])));
            ConsoleProgressBar("(Assimp) Materials", 45, i, scene->mNumMaterials);
        }

        std::cout << std::endl;

        // import material textures
        for(unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            ImportMaterialTextures(outScene, scene->mMaterials[i], outScene.materials[i]);
            ConsoleProgressBar("(Assimp) Textures ", 45, i, scene->mNumMaterials);
        }

        std::cout << std::endl;
    }

    if(scene->HasMeshes())
    {
        for(unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            outScene.meshes.push_back(std::move(ImportMesh(scene->mMeshes[i])));
            // material assigned to mesh
            outScene.meshes[i].material =
                &outScene.materials[scene->mMeshes[i]->mMaterialIndex];
            ConsoleProgressBar("(Assimp) Meshes   ", 45, i, scene->mNumMeshes);
        }
    }

    if(scene->mRootNode != nullptr)
    {
        ProcessNodes(outScene, scene->mRootNode, outScene.rootNode);
    }

    return true;
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

void SceneImporter::ProcessNodes(Scene &scene, aiNode* node, Node &newNode)
{
    newNode.name = node->mName.length > 0 ? node->mName.C_Str() : newNode.name;
    // transformation matrix
    newNode.transformation = glm::make_mat4(node->mTransformation[0]);

    // meshes associated with this node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        newNode.meshes.push_back(&scene.meshes[node->mMeshes[i]]);
    }

    // push childrens in hierachy
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        newNode.nodes.push_back(Node());
        ProcessNodes(scene, node->mChildren[i], newNode.nodes[i]);
    }
}

void SceneImporter::ImportMaterialTextures(Scene &scene, aiMaterial * mMaterial,
        Material &material)
{
    for(unsigned int texType = aiTextureType::aiTextureType_NONE;
        texType < aiTextureType::aiTextureType_UNKNOWN; texType++)
    {
        int textureTypeCount = mMaterial->GetTextureCount((aiTextureType)texType);

        // only loading one
        if(textureTypeCount <= 0) { continue; }

        aiString textureFilepath;

        if(mMaterial->GetTexture((aiTextureType)texType, 0,
                                 &textureFilepath) == AI_SUCCESS)
        {
            // find if texture was already loaded previously
            bool alreadyLoaded = false;

            for(unsigned int i = 0; i < scene.textures.size() && !alreadyLoaded; i++)
            {
                alreadyLoaded |= scene.textures[i].filepath == textureFilepath.data ?
                                 true : false;
            }

            if(!alreadyLoaded)
            {
                scene.textures.push_back(std::move(textureImporter.ImportTexture(
                                                       textureFilepath.data)));
                material.textures[texType] = &scene.textures.back();
            }
        }
    }
}
