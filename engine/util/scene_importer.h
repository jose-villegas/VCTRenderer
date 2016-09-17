#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <array>

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
        enum ImportFlags
        {
            CalculateTangentSpace = aiProcess_CalcTangentSpace,
            JoinIdenticalVertices = aiProcess_JoinIdenticalVertices,
            MakeLeftHanded = aiProcess_MakeLeftHanded,
            Triangulate = aiProcess_Triangulate,
            RemoveComponent = aiProcess_RemoveComponent,
            GenerateNormals = aiProcess_GenNormals,
            GenerateSmoothNormals = aiProcess_GenSmoothNormals,
            SplitLargeMeshes = aiProcess_SplitLargeMeshes,
            PreTransformVertices = aiProcess_PreTransformVertices,
            LimitBoneWeights = aiProcess_LimitBoneWeights,
            ValidateDataStructure = aiProcess_ValidateDataStructure,
            ImproveCacheLocality = aiProcess_ImproveCacheLocality,
            RemoveRedundantMaterials = aiProcess_RemoveRedundantMaterials,
            FixInfacingNormals = aiProcess_FixInfacingNormals,
            SortByType = aiProcess_SortByPType,
            FindDegenerates = aiProcess_FindDegenerates,
            FindInvalidData = aiProcess_FindInvalidData,
            GenerateUVCoords = aiProcess_GenUVCoords,
            TransformUVCoords = aiProcess_TransformUVCoords,
            FindInstances = aiProcess_FindInstances,
            OptimizeMeshes = aiProcess_OptimizeMeshes,
            OptimizeGraph = aiProcess_OptimizeGraph,
            FlipUVs = aiProcess_FlipUVs ,
            FlipWindingOrder = aiProcess_FlipWindingOrder,
            SplitByBoneCount = aiProcess_SplitByBoneCount,
            Debone = aiProcess_Debone,
        };
        static const std::array<const std::string, 26> FlagNames;
        /// <summary>
        /// Imports a scene from the specified filepath.
        /// </summary>
        /// <param name="filepath">The filepath.</param>
        /// <param name="scene">The scene.</param>
        /// <param name="flags">The flags.</param>
        /// <returns></returns>
        static bool Import(const std::string &filepath, Scene * scene, unsigned flags);

        SceneImporter();
        virtual ~SceneImporter();
    private:
        static void ImportMaterial(aiMaterial * mMaterial, Material &material);
        static void ImportMesh(aiMesh * mMesh, Mesh &mesh);
        static void ProcessNodes(Scene * scene, aiNode * mNode, Node &node);
        static void ImportCamera(aiCamera * mCam, Camera &camera);
        static void ImportLight(aiLight * mLight, Light &light);
        static void ImportMaterialTextures(Scene * scene, aiMaterial * mMaterial,
                                           Material &material);
};

