#pragma once
#include "interface.h"

class VCTInterface : public Interface
{
    private:
        std::vector<char *> availableScenes;
        int activeScene;
    public:
        void Draw();
        VCTInterface();
        virtual ~VCTInterface();
};

