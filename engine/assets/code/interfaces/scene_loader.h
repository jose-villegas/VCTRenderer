#pragma once

#include "../../../core/interface.h"

class EngineBase;

class UISceneLoader : public Interface
{
    protected:
        void Draw() override;
    public:
        UISceneLoader();
        ~UISceneLoader() override;
};
