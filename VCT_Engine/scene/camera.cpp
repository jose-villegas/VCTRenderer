#include "stdafx.h"
#include "camera.h"


Camera::Camera()
{
    clipPlaneFar = 0.0f;
    clipPlaneNear = 0.0f;
}


Camera::~Camera()
{
}

glm::mat4x4 &Camera::GetViewMatrix() const
{
    return glm::lookAt(this->position, this->lookAt, this->up);
}

glm::mat4x4 &Camera::GetProjecctionMatrix() const
{
    return glm::perspective(this->horizontalFoV, this->aspectRatio,
                            this->clipPlaneNear, this->clipPlaneFar);
}
