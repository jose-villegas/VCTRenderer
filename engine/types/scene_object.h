#pragma once

#include "transform.h"
#include "base_object.h"

class SceneObject : public BaseObject
{
    public:
        Transform transform;

        SceneObject();
        ~SceneObject();
};

