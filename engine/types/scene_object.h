#pragma once

#include "../scene/Transform.h"

class SceneObject : public BaseObject
{
    public:
        Transform transform;

        SceneObject();
        ~SceneObject();
};

