#pragma once

#include "transform.h"

class SceneObject : public Transform
{
    public:
        Transform &transform;

        SceneObject();
        ~SceneObject();

        SceneObject(const SceneObject &obj);  // copy constructor
};

