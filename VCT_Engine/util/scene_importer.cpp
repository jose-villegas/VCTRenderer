#include "stdafx.h"
#include "scene_importer.h"
#include "..\scene\scene.h"
#include "..\misc\miscellaneous.h"


SceneImporter::SceneImporter()
{
}


SceneImporter::~SceneImporter()
{
}

bool SceneImporter::Import(const std::string &sFilepath, Scene &outScene)
{
    std::cout << "(Assimp) Processing File: " << sFilepath << '\n';
    Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile(sFilepath,
                            aiProcessPreset_TargetRealtime_Fast);

    if(!scene)
    {
        std::cout << "(Assimp) Error Loading File: " << importer.GetErrorString() <<
                  std::endl;
        return false;
    }

    // location info
    outScene.filepath = sFilepath;
    outScene.directory = GetDirectoryPath(sFilepath);

    if(scene->HasMaterials())
    {
        // process material properties
        for(unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            Material * newMaterial = new Material();
            ImportMaterial(scene->mMaterials[i], *newMaterial);
            outScene.materials.push_back(newMaterial);
        }

        // import per material and scene, textures
        for(unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            ImportMaterialTextures(outScene, scene->mMaterials[i], *outScene.materials[i]);
        }
    }

    if(scene->HasMeshes())
    {
        for(unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            OGLMesh * newMesh = new OGLMesh();
            ImportMesh(scene->mMeshes[i], *newMesh);
            outScene.meshes.push_back(newMesh);
            // material assigned to mesh
            outScene.meshes[i]->material =
                outScene.materials[scene->mMeshes[i]->mMaterialIndex];
        }
    }

    if(scene->mRootNode != nullptr)
    {
        ProcessNodes(outScene, scene->mRootNode, outScene.rootNode);
    }

    std::cout << "(Assimp) Scene (.." << outScene.filepath.substr(
                  outScene.filepath.size() - 30, 30) << ") loaded successfully"
              << std::endl;
    return true;
}

void SceneImporter::ImportMaterial(aiMaterial *mMaterial, Material &outMaterial)
{
    // assimp scene material name extract
    aiString materialName;
    mMaterial->Get(AI_MATKEY_NAME, materialName);

    if(materialName.length > 0)
    {
        outMaterial.name = materialName.C_Str();
    }

    // material factors
    mMaterial->Get(AI_MATKEY_SHADING_MODEL, outMaterial.shadingModel);
    mMaterial->Get(AI_MATKEY_BLEND_FUNC, outMaterial.blendMode);
    mMaterial->Get(AI_MATKEY_REFRACTI, outMaterial.refractionIndex);
    mMaterial->Get(AI_MATKEY_SHININESS, outMaterial.shininess);
    mMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, outMaterial.shininessStrenght);
    // get material properties
    aiColor3D ambient(0.f), diffuse(0.f), specular(0.f);
    aiColor3D emissive(0.f), transparent(0.f);
    mMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    mMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    mMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    mMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
    mMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent);
    outMaterial.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
    outMaterial.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
    outMaterial.specular = glm::vec3(specular.r, specular.g, specular.b);
    outMaterial.emissive = glm::vec3(emissive.r, emissive.g, emissive.b);
    outMaterial.transparent = glm::vec3(transparent.r, transparent.g,
                                        transparent.b);
}
void SceneImporter::ImportMesh(aiMesh *mMesh, Mesh &outMesh)
{
    outMesh.name = mMesh->mName.length > 0 ? mMesh->mName.C_Str() : outMesh.name;

    if(mMesh->mNumVertices > 0)
    {
        for(unsigned int i = 0; i < mMesh->mNumVertices; i++)
        {
            outMesh.vertices.push_back(Vertex());
            Vertex * vertex = &outMesh.vertices.back();
            // store mesh data
            vertex->position = glm::vec3(mMesh->mVertices[i].x,
                                         mMesh->mVertices[i].y,
                                         mMesh->mVertices[i].z);
            vertex->normal = glm::vec3(mMesh->mNormals[i].x,
                                       mMesh->mNormals[i].y,
                                       mMesh->mNormals[i].z);
            vertex->uv = glm::vec2(mMesh->mTextureCoords[0]->x,
                                   mMesh->mTextureCoords[0]->y);

            if(mMesh->HasTangentsAndBitangents())
            {
                vertex->tangent = glm::vec3(mMesh->mTangents[i].x,
                                            mMesh->mTangents[i].y,
                                            mMesh->mTangents[i].z);
                vertex->bitangent = glm::vec3(mMesh->mBitangents[i].x,
                                              mMesh->mBitangents[i].y,
                                              mMesh->mBitangents[i].z);
            }

            vertex->Orthonormalize();
        }
    }

    for(unsigned int i = 0; i < mMesh->mNumFaces; i++)
    {
        outMesh.indices.push_back(mMesh->mFaces[i].mIndices[0]);
        outMesh.indices.push_back(mMesh->mFaces[i].mIndices[1]);
        outMesh.indices.push_back(mMesh->mFaces[i].mIndices[2]);
    }
}

void SceneImporter::ProcessNodes(Scene &scene, aiNode* node, Node &newNode)
{
    newNode.name = node->mName.length > 0 ? node->mName.C_Str() : newNode.name;
    // transformation matrix decomposition using assimp implementation
    aiVector3D position; aiVector3D scaling; aiQuaternion rotation;
    node->mTransformation.Decompose(scaling, rotation, position);
    newNode.position = glm::vec3(position.x, position.y, position.z);
    newNode.scaling = glm::vec3(scaling.x, scaling.y, scaling.z);
    newNode.rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);

    // meshes associated with this node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        newNode.meshes.push_back(scene.meshes[node->mMeshes[i]]);
    }

    // push childrens in hierachy
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        newNode.nodes.push_back(Node());
        ProcessNodes(scene, node->mChildren[i], newNode.nodes.back());
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

        aiString texPath;

        if(mMaterial->GetTexture((aiTextureType)texType, 0,
                                 &texPath) == AI_SUCCESS)
        {
            std::string filepath = scene.directory + "\\" + std::string(texPath.data);
            // find if texture was already loaded previously
            bool alreadyLoaded = false;
            int savedTextureIndex = 0;

            for(unsigned int i = 0; i < scene.textures.size() && !alreadyLoaded; ++i)
            {
                alreadyLoaded |= scene.textures[i]->GetFilepath() == filepath ? true : false;
                savedTextureIndex = i;
            }

            if(!alreadyLoaded)
            {
                OGLTexture2D * newTexture(new OGLTexture2D());

                if(textureImporter.ImportTexture2D(filepath, *newTexture))
                {
                    scene.textures.push_back(newTexture);
                    material.textures[texType] = newTexture;
                    newTexture->textureTypes.insert((RawTexture::TextureType)texType);
                }
            }
            else
            {
                material.textures[texType] = scene.textures[savedTextureIndex];
                material.textures[texType]->textureTypes.insert((RawTexture::TextureType)
                        texType);
            }
        }
    }
}
