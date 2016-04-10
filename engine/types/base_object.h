#pragma once
#include <string>
#include <bitset>
#include <array>
#include <vector>

class BaseObject
{
    public:
        std::string name;
        std::array<std::bitset<32>, 64> mode;
        BaseObject();
        virtual ~BaseObject();
};

