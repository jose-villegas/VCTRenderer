#pragma once
#include <vector>
#include <memory>
#include "../util/scene_importer.h"

class Scene;

/// <summary>
/// This class holds all the assets
/// used by the engine in runtime.
/// </summary>
class EngineAssets
{
    public:
        EngineAssets();
        virtual ~EngineAssets();
        std::vector<std::shared_ptr<Scene>> scenes;
        // No copying, copy, move assignment allowed of this class
        // or any derived class
        EngineAssets(EngineAssets const &r) = delete;
        EngineAssets(EngineAssets const &&r) = delete;
        EngineAssets &operator=(EngineAssets const &r) = delete;
};


