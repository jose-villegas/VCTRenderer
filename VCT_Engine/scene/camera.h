#pragma once
class Camera
{
	private:
		static Camera * mainCamera;

    public:
        Camera();
        virtual ~Camera();

        float clipPlaneFar;
        float clipPlaneNear;
        float horizontalFoV;
        float aspectRatio;
        glm::vec3 lookAt;
        glm::vec3 position;
        glm::vec3 up;

        glm::mat4x4 GetViewMatrix() const;
        glm::mat4x4 GetProjecctionMatrix() const;

		void SetMainCamera();
		static Camera * Main();
};

