#pragma once
#include "interface.h"

namespace VCT_ENGINE
{
    class UI : public Interface
    {
        public:
            void Draw();
            UI();
            virtual ~UI();
    };
}