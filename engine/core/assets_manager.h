#pragma once

#include <memory>
#include <map>

class Renderer;
class Behavior;
class ProgramShader;
class Scene;
class Interface;

/// <summary>
/// This class holds all the assets
/// used by the engine in runtime.
/// </summary>
class AssetsManager
{
    public:
        ~AssetsManager();
        /// <summary>
        /// Instances and initializes the assets manager.
        /// </summary>
        /// <returns></returns>
        static std::unique_ptr<AssetsManager> &Instance();
        /// <summary>
        /// Terminates this instance. Call at engine loop end
        /// </summary>
        static void Terminate();
        /// <summary>
        /// Contains pointers to the asset manager scenes
        /// </summary>
        std::map<std::string, std::shared_ptr<Scene>> scenes;
        /// <summary>
        /// Pointers to implemented interfaces
        /// </summary>
        std::map<std::string, std::shared_ptr<Interface>> interfaces;
        /// <summary>
        /// Pointers to implemented behaviors
        /// </summary>
        std::map<std::string, std::shared_ptr<Behavior>> behaviors;
        /// <summary>
        /// Pointers to implemented shader programs
        /// </summary>
        std::map<std::string, std::shared_ptr<ProgramShader>> programs;
        /// <summary>
        /// Pointers to implemented renderers
        /// </summary>
        std::map<std::string, std::shared_ptr<Renderer>> renderers;

        // No copying, copy, move assignment allowed of this class
        // or any derived class
        AssetsManager(AssetsManager const &r) = delete;
        AssetsManager(AssetsManager const &&r) = delete;
        AssetsManager &operator=(AssetsManager const &r) = delete;
    private:
        AssetsManager();
};


