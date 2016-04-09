#pragma once
#include "../../../core/interface.h"

class UISceneMaterials : public Interface
{
    protected:
        void Draw() override;
    public:
        UISceneMaterials();
        ~UISceneMaterials();
};

