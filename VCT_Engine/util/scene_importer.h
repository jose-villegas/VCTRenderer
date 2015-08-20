#pragma once
#include "..\scene\scene.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

class SceneImporter
{
    public:
        SceneImporter();
        virtual ~SceneImporter();
        bool Import(const std::string &sFilepath, Scene &outScene);
        // fine imports
        Material ImportMaterial(aiMaterial* mMaterial);
};

