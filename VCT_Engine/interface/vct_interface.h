#pragma once
#include "interface.h"

namespace VCT_ENGINE
{
    class UI : public Interface
    {
        private:
            std::vector<char *> availableScenes;
            int activeScene;
        public:
            void Draw();
            UI();
            virtual ~UI();
    };
}