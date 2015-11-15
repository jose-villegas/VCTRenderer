#include "stdafx.h"
#include "scene.h"

Scene::Scene()
{
    // scenes have by default at least one camera
    // they will be removed if camera or light info
    // is found during scene extracction
    this->cameras.push_back(std::make_shared<Camera>());
    this->lights.push_back(std::make_shared<Light>());
}
