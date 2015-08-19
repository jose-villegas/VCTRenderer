#pragma once
#include "camera.h"
#include "light.h"
#include "material.h"
#include "mesh.h"
#include "texture.h"
class Scene
{
    public:
        std::vector<Camera> cameras;
        std::vector<Light> lights;
        std::vector<Material> materials;
        std::vector<Mesh> meshes;
        std::vector<Texture> textures;
        Scene();
        virtual ~Scene();
};

