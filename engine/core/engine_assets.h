#pragma once

#include <vector>
#include <memory>

class Scene;
class Interface;

/// <summary>
/// This class holds all the assets
/// used by the engine in runtime.
/// </summary>
class AssetsManager
{
    public:
        virtual ~AssetsManager();
        static std::unique_ptr<AssetsManager> &Instance();
        std::vector<std::shared_ptr<Scene>> scenes;
        std::vector<std::shared_ptr<Interface>> interfaces;

        static void Terminate();
        // No copying, copy, move assignment allowed of this class
        // or any derived class
        AssetsManager(AssetsManager const &r) = delete;
        AssetsManager(AssetsManager const &&r) = delete;
        AssetsManager &operator=(AssetsManager const &r) = delete;
    private:
        AssetsManager();
};


