#pragma once
#include "../../../core/interface.h"

class UISceneMaterials : public Interface
{
    protected:
        void Draw() override;
    public:
        UISceneMaterials() = default;
        ~UISceneMaterials() = default;
};

class UISceneNodes : public Interface
{
    protected:
        void Draw() override;
    public:
        UISceneNodes() = default;
        ~UISceneNodes() = default;
};

