#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <memory>
#include <glm/gtc/quaternion.hpp>

#include "scene_importer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "texture_importer.h"
#include "../scene/scene.h"
#include "../scene/material.h"
#include "../scene/mesh.h"
#include "../scene/camera.h"
#include "../scene/light.h"
#include "../types/vertex.h"

SceneImporter::SceneImporter()
{
}


SceneImporter::~SceneImporter()
{
}

bool SceneImporter::Import(const std::string &filepath, Scene * scene)
{
    Assimp::Importer importer;
    const aiScene * mScene = importer.ReadFile(filepath,
                             aiProcessPreset_TargetRealtime_MaxQuality);

    if (!mScene)
    {
        throw std::runtime_error(importer.GetErrorString());
    }

    if (mScene->HasMaterials())
    {
        scene->materials.clear();

        // process material properties
        for (unsigned int i = 0; i < mScene->mNumMaterials; i++)
        {
            std::shared_ptr<Material> newMaterial(new Material());
            ImportMaterial(mScene->mMaterials[i], *newMaterial);
            scene->materials.push_back(newMaterial);
        }

        scene->textures.clear();

        // import per material and scene, textures
        for (unsigned int i = 0; i < mScene->mNumMaterials; i++)
        {
            ImportMaterialTextures(scene, mScene->mMaterials[i],
                                   *scene->materials[i]);
        }
    }

    if (mScene->HasMeshes())
    {
        scene->meshes.clear();

        for (unsigned int i = 0; i < mScene->mNumMeshes; i++)
        {
            std::shared_ptr<MeshDrawer> newMesh(new MeshDrawer());
            ImportMesh(mScene->mMeshes[i], *newMesh);
            // material assigned to mesh
            newMesh->material = scene->materials
                                [mScene->mMeshes[i]->mMaterialIndex];
            scene->meshes.push_back(move(newMesh));
        }
    }

    if (mScene->mRootNode != nullptr)
    {
        ProcessNodes(scene, mScene->mRootNode, *scene->rootNode);
    }

    // if these objects don't exist in scene, they are created by default
    if (mScene->HasCameras())
    {
        scene->cameras.clear();

        for (unsigned int i = 0; i < mScene->mNumCameras; i++)
        {
            std::shared_ptr<Camera> newCamera = std::make_shared<Camera>();
            ImportCamera(mScene->mCameras[i], *newCamera);
            scene->cameras.push_back(newCamera);
        }
    }

    if (mScene->HasLights())
    {
        scene->lights.clear();

        for (unsigned int i = 0; i < mScene->mNumLights; i++)
        {
            std::shared_ptr<Light> newLight = std::make_shared<Light>();
            ImportLight(mScene->mLights[i], *newLight);
            scene->lights.push_back(newLight);
        }
    }

    importer.FreeScene();
    return true;
}

void SceneImporter::ImportLight(aiLight * mLight, Light &light)
{
    light.name = mLight->mName.length > 0 ? mLight->mName.C_Str() : light.name;
    light.Ambient(glm::vec3(
                      mLight->mColorAmbient.r,
                      mLight->mColorAmbient.g,
                      mLight->mColorAmbient.b
                  ));
    light.Diffuse(glm::vec3(
                      mLight->mColorDiffuse.r,
                      mLight->mColorDiffuse.g,
                      mLight->mColorDiffuse.b
                  ));
    light.Specular(glm::vec3(
                       mLight->mColorSpecular.r,
                       mLight->mColorSpecular.g,
                       mLight->mColorSpecular.b
                   ));
    light.transform.Forward(glm::vec3(
                                mLight->mDirection.x,
                                mLight->mDirection.y,
                                mLight->mDirection.z
                            ));
    light.transform.Position(glm::vec3(
                                 mLight->mPosition.x,
                                 mLight->mPosition.y,
                                 mLight->mPosition.z
                             ));
    light.TypeCollection(mLight->mType == aiLightSource_POINT
                         ? Light::Point
                         : mLight->mType == aiLightSource_DIRECTIONAL
                         ? Light::Directional
                         : mLight->mType == aiLightSource_SPOT
                         ? Light::Spot
                         : Light::Point
                        );
    light.AngleInnerCone(mLight->mAngleInnerCone);
    light.AngleOuterCone(mLight->mAngleOuterCone);
    light.attenuation.Constant(mLight->mAttenuationConstant);
    light.attenuation.Linear(mLight->mAttenuationLinear);
    light.attenuation.Quadratic(mLight->mAttenuationQuadratic);
}

void SceneImporter::ImportCamera(aiCamera * mCam, Camera &camera)
{
    camera.name = mCam->mName.length > 0 ? mCam->mName.C_Str() : camera.name;
    camera.AspectRatio(mCam->mAspect);
    camera.ClipPlaneFar(mCam->mClipPlaneFar);
    camera.ClipPlaneNear(mCam->mClipPlaneNear);
    camera.FieldOfView(mCam->mHorizontalFOV);
    auto lookat = glm::vec3(mCam->mLookAt.x, mCam->mLookAt.y, mCam->mLookAt.z);
    auto pos = glm::vec3(mCam->mPosition.x, mCam->mPosition.y, mCam->mPosition.z);
    auto up = glm::vec3(mCam->mUp.x, mCam->mUp.y, mCam->mUp.z);
    auto fwd = normalize(lookat - pos);
    camera.transform.Position(pos);
    camera.transform.Forward(fwd);
    camera.transform.Up(up);
    // latest created always as active
    camera.SetAsActive();
}

void SceneImporter::ImportMaterial(aiMaterial * mMaterial,
                                   Material &material)
{
    // assimp scene material name extract
    aiString matName;
    mMaterial->Get(AI_MATKEY_NAME, matName);
    material.name = matName.length > 0 ? matName.C_Str() : material.name;
    float refracti, shininess, shinStrength;
    // material factors
    mMaterial->Get(AI_MATKEY_REFRACTI, refracti);
    mMaterial->Get(AI_MATKEY_SHININESS, shininess);
    mMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, shinStrength);
    material.RefractionIndex(refracti);
    material.Shininess(shininess / 1024.0f);
    material.ShininessStrenght(shinStrength);
    // get material properties
    aiColor3D ambient(0.f), diffuse(0.f), specular(0.f);
    aiColor3D emissive(0.f), transparent(0.f);
    mMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    mMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    mMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    mMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
    mMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent);
    material.Ambient(glm::vec3(ambient.r, ambient.g, ambient.b));
    material.Diffuse(glm::vec3(diffuse.r, diffuse.g, diffuse.b));
    material.Specular(glm::vec3(specular.r, specular.g, specular.b));
    material.Emissive(glm::vec3(emissive.r, emissive.g, emissive.b));
    material.Transparent(glm::vec3(transparent.r, transparent.g,
                                   transparent.b));
}

void SceneImporter::ImportMesh(aiMesh * mMesh, Mesh &mesh)
{
    mesh.name = mMesh->mName.length > 0 ? mMesh->mName.C_Str() : mesh.name;

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
            mesh.boundaries.MinPoint(vertex.position);
            mesh.boundaries.MaxPoint(vertex.position);
            // gram-schmidt orthonormalization
            vertex.Orthonormalize();
            // new vertex to raw mesh data
            mesh.vertices.push_back(std::move(vertex));
        }
    }

    for (unsigned int i = 0; i < mMesh->mNumFaces; i++)
    {
        mesh.indices.push_back(mMesh->mFaces[i].mIndices[0]);
        mesh.indices.push_back(mMesh->mFaces[i].mIndices[1]);
        mesh.indices.push_back(mMesh->mFaces[i].mIndices[2]);
    }
}

void SceneImporter::ProcessNodes(Scene * scene, aiNode * mNode, Node &node)
{
    node.name = mNode->mName.length > 0 ? mNode->mName.C_Str() : node.name;

    // meshes associated with this node
    for (unsigned int i = 0; i < mNode->mNumMeshes; i++)
    {
        // insert after same name
        node.meshes.push_back(scene->meshes[mNode->mMeshes[i]]);
        // node boundaries based on mesh boundaries
        node.boundaries.MinPoint
        (
            scene->meshes[mNode->mMeshes[i]]->boundaries.MinPoint()
        );
        node.boundaries.MaxPoint
        (
            scene->meshes[mNode->mMeshes[i]]->boundaries.MaxPoint()
        );
    }

    // push childrens in hierachy
    for (unsigned int i = 0; i < mNode->mNumChildren; i++)
    {
        // create children
        node.nodes.push_back(std::make_shared<Node>());
        ProcessNodes(scene, mNode->mChildren[i], *node.nodes.back());
        // node boundaries based on children node boundaries
        node.boundaries.MinPoint
        (
            node.nodes.back()->boundaries.MinPoint()
        );
        node.boundaries.MaxPoint
        (
            node.nodes.back()->boundaries.MaxPoint()
        );
    }

    // transformation matrix decomposition using assimp implementation
    aiVector3D pos; aiVector3D sca; aiQuaternion rot;
    mNode->mTransformation.Decompose(sca, rot, pos);
    node.transform.Position(glm::vec3(pos.x, pos.y, pos.z));
    node.transform.Scale(glm::vec3(sca.x, sca.y, sca.z));
    node.transform.Rotation(glm::quat(rot.w, rot.x, rot.y, rot.z));
    // build per node draw lists from recursive draw
    // useful for easier batching
    node.BuildDrawList();
}

inline std::string GetFileExtension(const std::string &sFilepath)
{
    auto result = sFilepath.substr(sFilepath.find_last_of(".") + 1);
    return result == sFilepath ? "" : result;
}

void SceneImporter::ImportMaterialTextures(Scene * scene,
        aiMaterial * mMaterial,
        Material &material)
{
    for (aiTextureType texType = aiTextureType_NONE;
            texType < aiTextureType_UNKNOWN;
            texType = aiTextureType(texType + 1))
    {
        int textureTypeCount = mMaterial->GetTextureCount(static_cast<aiTextureType>
                               (texType));

        // only loading one
        if (textureTypeCount <= 0) { continue; }

        aiString texPath;

        if (mMaterial->GetTexture(static_cast<aiTextureType>(texType), 0,
                                  &texPath) == AI_SUCCESS)
        {
            auto filepath = scene->directory + "\\" + std::string(texPath.data);
            // find if texture was already loaded previously
            bool alreadyLoaded = false;
            int savedTextureIndex = 0;

            // for wavefront obj we assimp bump = normal map
            if (GetFileExtension(scene->filepath) == "obj" &&
                    texType == aiTextureType_HEIGHT)
            { texType = aiTextureType_NORMALS; }

            for (unsigned int i = 0; i < scene->textures.size() && !alreadyLoaded; ++i)
            {
                alreadyLoaded |= scene->textures[i]->GetFilepath() == filepath;
                savedTextureIndex = i;
            }

            if (!alreadyLoaded)
            {
                auto newTexture = std::make_shared<Texture2D>();

                if (TextureImporter::ImportTexture2D(filepath, *newTexture))
                {
                    scene->textures.push_back(newTexture);
                    material.AddTexture
                    (
                        newTexture,
                        static_cast<RawTexture::TextureType>(texType)
                    );
                    newTexture->textureTypes.insert
                    (
                        static_cast<RawTexture::TextureType>(texType)
                    );
                }
                else
                {
                    throw std::runtime_error("Error loading texture " + filepath);
                }
            } // raw data from this texture has been previously loaded
            else
            {
                // just add reference and associate a new texture type
                material.AddTexture
                (
                    scene->textures[savedTextureIndex],
                    static_cast<RawTexture::TextureType>(texType)
                );
                scene->textures[savedTextureIndex]->textureTypes.insert
                (
                    static_cast<RawTexture::TextureType>(texType)
                );
            }
        }
    }
}
