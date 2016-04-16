#pragma once
#include "../../scene/mesh.h"
#include "../../scene/scene.h"

class Shapes
{
    public:
        static std::vector<std::string> ShapeNameList();
        static std::shared_ptr<Node> GetShape(const std::string &name);
        // this will remove shapes from memory since they are
        // context dependent they have to be deleted before closing the context
        static void UnloadShapes();
        static void Load();
    private:
        static std::unordered_map<std::string, std::shared_ptr<Node>> shapes;
};

