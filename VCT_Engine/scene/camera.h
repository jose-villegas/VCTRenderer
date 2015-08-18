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
};

