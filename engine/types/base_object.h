#pragma once
#include <string>
#include <bitset>
#include <array>

class BaseObject
{
    public:
        std::string name;
        std::array<std::bitset<32>, 64> mode;
        BaseObject();
        virtual ~BaseObject();
};

