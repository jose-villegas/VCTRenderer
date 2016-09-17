#pragma once
#include <string>
#include <bitset>
#include <array>
#include <vector>

class BaseObject
{
    public:
        /// <summary>
        /// The object name
        /// </summary>
        std::string name;
        /// <summary>
        /// The object flags
        /// </summary>
        std::array<std::bitset<32>, 64> mode;

        BaseObject();
        virtual ~BaseObject();
};

