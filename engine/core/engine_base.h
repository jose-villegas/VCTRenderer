#pragma once

#include <memory>

class RenderWindow;
class DeferredRenderer;
class AssetsManager;

/// <summary>
/// This is the entry point of the rendering engine
/// where the main rendering loop resides and the
/// rendering context is properly set up.
/// </summary>
class EngineBase
{
    public:
        virtual ~EngineBase();
        /// <summary>
        /// Main rendering loop
        /// </summary>
        void MainLoop() const;
        /// <summary>
        /// Returns the EngineBase singleton instance.
        /// </summary>
        /// <returns></returns>
        static std::unique_ptr<EngineBase> &Instance();
        /// <summary>
        /// Terminates this instance.
        /// </summary>
        static void Terminate();
        // No copying, copy, move assignment allowed of this class
        // or any derived class
        EngineBase(EngineBase const &r) = delete;
        EngineBase(EngineBase const &&r) = delete;
        EngineBase &operator=(EngineBase const &r) = delete;
    private:
        /// <summary>
        /// The rendering window.
        /// </summary>
        std::unique_ptr<RenderWindow> renderWindow;
        /// <summary>
        /// Setups all the engine components, imports assets
        /// and initializes libraries.
        /// </summary>
        void Initialize() const;
        EngineBase();
};
