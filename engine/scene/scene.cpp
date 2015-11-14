#include "stdafx.h"
#include "scene.h"

Scene::Scene()
{
    // scenes have by default at least one camera
    // and a light, they can be deleted
    this->cameras.push_back(std::make_shared<Camera>());
    this->lights.push_back(std::make_shared<Light>());
}


Scene::~Scene()
{
}
