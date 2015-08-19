#pragma once
class Camera
{
    public:
        Camera();
        virtual ~Camera();
    private:
        float clipPlaneFar;
        float clipPlaneNear;
        float horizontalFoV;
        glm::vec3 lookAt;
        glm::vec3 position;
        glm::vec3 up;
};

