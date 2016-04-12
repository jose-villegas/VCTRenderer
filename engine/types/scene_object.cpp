#include "scene_object.h"

SceneObject::SceneObject() : transform(*this)
{
}

SceneObject::~SceneObject()
{
}

SceneObject::SceneObject(const SceneObject &obj) : transform(*this)
{
    this->name = obj.name;
}
