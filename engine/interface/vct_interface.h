#pragma once

#include "interface.h"
#include <oglplus/texture.hpp>

class EngineBase;

class UI : public Interface
{
    public:
        void DrawGBufferTexture(const oglplus::Texture &texture,
                                const std::string &name) const;
        void DrawFramerateMetrics() const;
        void DrawSceneSelector();
        void DrawGBufferTextures() const;
        void DrawDebugWindow() const;
        void DrawFPSNotif() const;
        void Draw() override;
        UI();
        virtual ~UI();
    private:
        ImVec2 uiPos, uiSize;
};
