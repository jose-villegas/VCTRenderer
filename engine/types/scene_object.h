#pragma once

#include "transform.h"

/// <summary>
/// Use for objects that residen within a scene
/// </summary>
/// <seealso cref="Transform" />
class SceneObject : public Transform
{
    public:
        /// <summary>
        /// The object's transform
        /// </summary>
        Transform &transform;

        SceneObject();
        ~SceneObject();
        SceneObject(const SceneObject &obj);
};

