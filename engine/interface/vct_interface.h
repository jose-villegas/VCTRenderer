#pragma once
#include "interface.h"

class UI : public Interface
{


    public:
        void DrawGBufferTexture(const oglplus::Texture &texture,
                                const std::string &name) const;
        void DrawFramerateMetrics() const;
        void DrawSceneSelector();
        void DrawGBufferTextures();
        void DrawDebugWindow();
        void DrawFPSNotif() const;
        void Draw() override;
        UI();
        virtual ~UI();
    private:
        ImVec2 uiPos, uiSize;
        ImGuiIO &io;
};
