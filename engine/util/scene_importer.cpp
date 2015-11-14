#include "stdafx.h"
#include "scene_importer.h"
#include "../scene/scene.h"
#include "../misc/utils.h"
#include "scene/texture.h"


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

    if (!scene)
    {
        std::cout << "(Assimp) Error Loading File: " << importer.GetErrorString() <<
                  std::endl;
        return false;
    }

    // location info
    outScene.filepath = sFilepath;
    outScene.directory = utils::GetDirectoryPath(sFilepath);

    if (scene->HasMaterials())
    {
        // process material properties
        for (unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            std::shared_ptr<OGLMaterial> newMaterial(new OGLMaterial());
            ImportMaterial(scene->mMaterials[i], *newMaterial);
            outScene.materials.push_back(newMaterial);
        }

        // import per material and scene, textures
        for (unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            ImportMaterialTextures(outScene, scene->mMaterials[i], *outScene.materials[i]);
        }
    }

    if (scene->HasMeshes())
    {
        for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            std::shared_ptr<OGLMesh> newMesh(new OGLMesh());
            ImportMesh(scene->mMeshes[i], *newMesh);
            // material assigned to mesh
            newMesh->material = outScene.materials[scene->mMeshes[i]->mMaterialIndex];
            // sort the meshes by material to reduce gl calls
            auto it = std::lower_bound(
                          outScene.meshes.begin(), outScene.meshes.end(), newMesh,
                          [ = ](std::shared_ptr<OGLMesh> lhs, std::shared_ptr<OGLMesh> rhs)
                          -> bool { return lhs->material->name < rhs->material->name; }
                      );
            outScene.meshes.insert(it, std::move(newMesh));
        }
    }

    if (scene->mRootNode != nullptr)
    {
        ProcessNodes(outScene, scene->mRootNode, outScene.rootNode);
    }

    // if these objects don't exist in scene, they are created by default
    if (scene->HasCameras())
    {
        outScene.cameras.clear();

        for (unsigned int i = 0; i < scene->mNumCameras; i++)
        {
            std::shared_ptr<Camera> newCamera = std::make_shared<Camera>();
            ImportCamera(scene->mCameras[i], *newCamera);
            outScene.cameras.push_back(newCamera);
        }
    }

    if (scene->HasLights())
    {
        outScene.lights.clear();

        for (unsigned int i = 0; i < scene->mNumLights; i++)
        {
            std::shared_ptr<Light> newLight = std::make_shared<Light>();
            ImportLight(scene->mLights[i], *newLight);
            outScene.lights.push_back(newLight);
        }
    }

    return true;
}

void SceneImporter::ImportLight(aiLight * light, Light &newLight)
{
    newLight.ambient = glm::vec3(
                           light->mColorAmbient.r,
                           light->mColorAmbient.g,
                           light->mColorAmbient.b
                       );
    newLight.diffuse = glm::vec3(
                           light->mColorDiffuse.r,
                           light->mColorDiffuse.g,
                           light->mColorDiffuse.b
                       );
    newLight.specular = glm::vec3(
                            light->mColorSpecular.r,
                            light->mColorSpecular.g,
                            light->mColorSpecular.b
                        );
    newLight.direction = glm::vec3(
                             light->mDirection.x,
                             light->mDirection.y,
                             light->mDirection.z
                         );
    newLight.position = glm::vec3(
                            light->mPosition.x,
                            light->mPosition.y,
                            light->mPosition.z
                        );
    newLight.lightType = light->mType == aiLightSourceType::aiLightSource_POINT ?
                         Light::Point : light->mType == aiLightSourceType::aiLightSource_DIRECTIONAL ?
                         Light::Directional : light->mType == aiLightSourceType::aiLightSource_SPOT ?
                         Light::Spot : Light::Point;
    newLight.angleInnerCone = light->mAngleInnerCone;
    newLight.angleOuterCone = light->mAngleOuterCone;
    newLight.attenuation.constant = light->mAttenuationConstant;
    newLight.attenuation.linear = light->mAttenuationLinear;
    newLight.attenuation.quadratic = light->mAttenuationQuadratic;
}

void SceneImporter::ImportCamera(aiCamera * cam, Camera &newCamera)
{
    newCamera.aspectRatio = cam->mAspect;
    newCamera.clipPlaneFar = cam->mClipPlaneFar;
    newCamera.clipPlaneNear = cam->mClipPlaneNear;
    newCamera.horizontalFoV = cam->mHorizontalFOV;
    newCamera.position = glm::vec3(
                             cam->mPosition.x,
                             cam->mPosition.y,
                             cam->mPosition.z
                         );
    newCamera.lookAt = glm::vec3(
                           cam->mLookAt.x,
                           cam->mLookAt.y,
                           cam->mLookAt.z
                       );
    newCamera.up = glm::vec3(
                       cam->mUp.x,
                       cam->mUp.y,
                       cam->mUp.z
                   );
}

void SceneImporter::ImportMaterial(aiMaterial * mMaterial,
                                   OGLMaterial &outMaterial)
{
    // assimp scene material name extract
    aiString materialName;
    mMaterial->Get(AI_MATKEY_NAME, materialName);

    if (materialName.length > 0)
    {
        outMaterial.name = materialName.C_Str();
    }

    // material factors
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
void SceneImporter::ImportMesh(aiMesh * mMesh, Mesh &outMesh)
{
    outMesh.name = mMesh->mName.length > 0 ? mMesh->mName.C_Str() : outMesh.name;

    if (mMesh->mNumVertices > 0)
    {
        for (unsigned int i = 0; i < mMesh->mNumVertices; i++)
        {
            Vertex vertex;
            // store mesh data
            vertex.position = glm::vec3(
                                  mMesh->mVertices[i].x,
                                  mMesh->mVertices[i].y,
                                  mMesh->mVertices[i].z
                              );
            vertex.normal = glm::vec3(
                                mMesh->mNormals[i].x,
                                mMesh->mNormals[i].y,
                                mMesh->mNormals[i].z
                            );

            if (mMesh->HasTextureCoords(0))
            {
                vertex.uv = glm::vec3(
                                mMesh->mTextureCoords[0][i].x,
                                mMesh->mTextureCoords[0][i].y,
                                mMesh->mTextureCoords[0][i].z
                            );
            }

            if (mMesh->HasTangentsAndBitangents())
            {
                vertex.tangent = glm::vec3(
                                     mMesh->mTangents[i].x,
                                     mMesh->mTangents[i].y,
                                     mMesh->mTangents[i].z
                                 );
                vertex.bitangent = glm::vec3(
                                       mMesh->mBitangents[i].x,
                                       mMesh->mBitangents[i].y,
                                       mMesh->mBitangents[i].z
                                   );
            }

            // update boundaries with current position
            outMesh.boundaries.TryMinMax(vertex.position, vertex.position);
            // gram-schmidt orthonormalization
            vertex.Orthonormalize();
            // new vertex to raw mesh data
            outMesh.vertices.push_back(std::move(vertex));
        }
    }

    for (unsigned int i = 0; i < mMesh->mNumFaces; i++)
    {
        outMesh.indices.push_back(mMesh->mFaces[i].mIndices[0]);
        outMesh.indices.push_back(mMesh->mFaces[i].mIndices[1]);
        outMesh.indices.push_back(mMesh->mFaces[i].mIndices[2]);
    }
}

void SceneImporter::ProcessNodes(Scene &scene, aiNode * node, Node &newNode)
{
    newNode.name = node->mName.length > 0 ? node->mName.C_Str() : newNode.name;
    // transformation matrix decomposition using assimp implementation
    aiVector3D position; aiVector3D scaling; aiQuaternion rotation;
    node->mTransformation.Decompose(scaling, rotation, position);
    newNode.Position(glm::vec3(position.x, position.y, position.z));
    newNode.Scaling(glm::vec3(scaling.x, scaling.y, scaling.z));
    newNode.Rotation(glm::quat(rotation.w, rotation.x, rotation.y, rotation.z));

    // meshes associated with this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        newNode.meshes.push_back(scene.meshes[node->mMeshes[i]]);
        // node boundaries based on mesh boundaries
        newNode.boundaries.TryMinMax(
            scene.meshes[node->mMeshes[i]]->boundaries.minPoint,
            scene.meshes[node->mMeshes[i]]->boundaries.maxPoint
        );
    }

    // push childrens in hierachy
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        newNode.nodes.push_back(std::move(Node()));
        ProcessNodes(scene, node->mChildren[i], newNode.nodes.back());
        // node boundaries based on children node boundaries
        newNode.boundaries.TryMinMax(
            newNode.nodes.back().boundaries.minPoint,
            newNode.nodes.back().boundaries.maxPoint
        );
    }
}

void SceneImporter::ImportMaterialTextures(Scene &scene, aiMaterial * mMaterial,
        OGLMaterial &material)
{
    bool materialHasTexture = false;

    for (aiTextureType texType = aiTextureType::aiTextureType_NONE;
         texType < aiTextureType::aiTextureType_UNKNOWN;
         texType = aiTextureType(texType + 1))
    {
        int textureTypeCount = mMaterial->GetTextureCount((aiTextureType)texType);

        // only loading one
        if (textureTypeCount <= 0) { continue; }

        aiString texPath;
        // material has at least one texture
        materialHasTexture = true;

        if (mMaterial->GetTexture((aiTextureType)texType, 0,
                                  &texPath) == AI_SUCCESS)
        {
            std::string filepath = scene.directory + "\\" + std::string(texPath.data);
            // find if texture was already loaded previously
            bool alreadyLoaded = false;
            int savedTextureIndex = 0;

            // for wavefront obj we assump bump = normal map
            if (GetExtension(scene.filepath) == "obj" &&
                texType == aiTextureType::aiTextureType_HEIGHT)
            { texType = aiTextureType::aiTextureType_NORMALS; }

            for (unsigned int i = 0; i < scene.textures.size() && !alreadyLoaded; ++i)
            {
                alreadyLoaded |= scene.textures[i]->GetFilepath() == filepath
                                 ? true
                                 : false;
                savedTextureIndex = i;
            }

            if (!alreadyLoaded)
            {
                std::shared_ptr<OGLTexture2D> newTexture(new OGLTexture2D());

                if (textureImporter.ImportTexture2D(filepath, *newTexture))
                {
                    scene.textures.push_back(newTexture);
                    material.AddTexture(newTexture, (RawTexture::TextureType)texType);
                    newTexture->textureTypes.insert((RawTexture::TextureType)texType);
                }
                else
                {
                    std::cout << "(SceneImporter) Error Loading Texture: " << filepath << std::endl;
                }
            } // raw data from this texture has been previously loaded
            else
            {
                // just add reference and associate a new texture type
                material.AddTexture(scene.textures[savedTextureIndex],
                                    (RawTexture::TextureType)texType);
                scene.textures[savedTextureIndex]->textureTypes.insert
                ((RawTexture::TextureType)texType);
            }
        }
    }
}
