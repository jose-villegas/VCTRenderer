#pragma once
#include <memory>

#include "../interface/vct_interface.h"
#include "render_window.h"

class DeferredRenderer;
class EngineAssets;

/// <summary>
/// This is the entry point of the rendering engine
/// where the main rendering loop resides.
/// </summary>
class EngineBase
{
    public:
        virtual ~EngineBase();

        /// <summary>
        /// Main rendering loop
        /// </summary>
        void MainLoop();

        /// <summary>
        /// Returns the EngineBase singleton instance.
        /// </summary>
        /// <returns></returns>
        static std::shared_ptr<EngineBase> &Instance();

        static UI &Interface() { return Instance()->userInterface; }
        static RenderWindow &Window() { return Instance()->renderWindow; }
        static EngineAssets &Assets() { return *Instance()->assets; }
        static DeferredRenderer &Renderer() { return *Instance()->renderer; }

        // No copying, copy, move assignment allowed of this class
        // or any derived class
        EngineBase(EngineBase const &r) = delete;
        EngineBase(EngineBase const &&r) = delete;
        EngineBase &operator=(EngineBase const &r) = delete;
    private:
        /// <summary>
        /// The user interface, renderer per frame. Immediate mode
        /// </summary>
        UI userInterface;
        /// <summary>
        /// The rendering window.
        /// </summary>
        RenderWindow renderWindow;
        /// <summary>
        /// The engine renderer, uses deferred rendering path.
        /// </summary>
        std::unique_ptr<DeferredRenderer> renderer;
        /// <summary>
        /// All the engine available assets.
        /// </summary>
        std::unique_ptr<EngineAssets> assets;

        /// <summary>
        /// Setups all the engine components, imports assets
        /// and initializes libraries.
        /// </summary>
        void Initialize();

        EngineBase();
};
