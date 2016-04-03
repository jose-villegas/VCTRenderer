#pragma once

#include "../util/single_active.h"
#include "../types/frustum.h"
#include "../types/scene_object.h"

/// <summary>
/// Holds parameters and settings for scene cameras.
/// Viewing parameters and projection setup for the camera instance
/// can be all modified here.
/// </summary>
/// <seealso cref="BaseObject" />
/// <seealso cref="SingleActive{Camera}" />
class Camera : public SceneObject, public SingleActive<Camera>
{
    public:
        void UpdateTransformMatrix() override;

        enum class ProjectionMode
        {
            Perspective,
            Orthographic
        };

        Camera();
        virtual ~Camera();

        void Projection(ProjectionMode mode);
        float ClipPlaneFar() const;
        /// <summary>
        /// Sets the <see cref="clipPlaneFar"> value.
        /// Value is in the range [0.01, inf]
        /// </summary>
        /// <param name="val">The value.</param>
        void ClipPlaneFar(float val);

        float ClipPlaneNear() const;
        /// <summary>
        /// Sets the <see cref="clipPlaneNear"> value.
        /// Value is in the range [0.01, inf]
        /// </summary>
        /// <param name="val">The value.</param>
        void ClipPlaneNear(float val);

        const float &FieldOfView() const;
        /// <summary>
        /// Sets the <see cref="fieldOfView"> value.
        /// Value is in the range [1, 179]
        /// </summary>
        /// <param name="val">The value.</param>
        void FieldOfView(const float &val);

        float AspectRatio() const;
        void AspectRatio(float val);

        // left right bottom top
        void OrthoRect(const glm::vec4 &rect);
        const glm::vec4 &OrthoRect() const;

        glm::vec3 LookAt() const;

        const glm::mat4x4 &ViewMatrix() const;
        const glm::mat4x4 &ProjectionMatrix() const;
        const glm::mat4x4 &ViewProjectionMatrix() const;
        const glm::mat4x4 &InverseViewMatrix() const;
        const glm::mat4x4 &InverseProjectionMatrix() const;

        /// <summary>
        /// Checks if the bounding volume is inside the
        /// camera frustum <see cref="frustum"/>
        /// </summary>
        /// <param name="volume">The volume.</param>
        /// <returns></returns>
        bool InFrustum(const BoundingBox &volume) const;
    private:
        float clipPlaneFar;
        float clipPlaneNear;
        float fieldOfView;
        float aspectRatio;
        glm::vec4 orthoRect;
        ProjectionMode mode;

        glm::mat4x4 viewMatrix;
        glm::mat4x4 projectionMatrix;
        glm::mat4x4 viewProjectionMatrix;
        glm::mat4x4 inverseViewMatrix;
        glm::mat4x4 inverseProjectionMatrix;
        CullingFrustum frustum;

        void UpdateProjectionMatrix();
};

