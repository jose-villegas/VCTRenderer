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
        Camera();
        virtual ~Camera();

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

        float HorizontalFoV() const;
        /// <summary>
        /// Sets the <see cref="horizontalFoV"> value.
        /// Value is in the range [1, 179]
        /// </summary>
        /// <param name="val">The value.</param>
        void HorizontalFoV(float val);

        float AspectRatio() const;
        void AspectRatio(float val);

        glm::vec3 LookAt() const;

        const glm::mat4x4 &ViewMatrix();
        const glm::mat4x4 &ProjectionMatrix();
        const glm::mat4x4 &InverseViewMatrix();
        const glm::mat4x4 &InverseProjectionMatrix();

        /// <summary>
        /// Checks if the bounding volume is inside the
        /// camera frustum <see cref="frustum"/>
        /// </summary>
        /// <param name="volume">The volume.</param>
        /// <returns></returns>
        bool InFrustum(const BoundingBox &volume);
        float clipPlaneFar;
        float clipPlaneNear;
        float horizontalFoV;
        float aspectRatio;

        glm::mat4x4 viewMatrix;
        glm::mat4x4 projectionMatrix;
        glm::mat4x4 inverseViewMatrix;
        glm::mat4x4 inverseProjectionMatrix;
        CullingFrustum frustum;
};

