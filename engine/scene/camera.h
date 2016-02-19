#pragma once

#include "../util/single_active.h"
#include "../types/frustum.h"
#include "../types/scene_object.h"

#include <glm/detail/type_vec3.hpp>

/// <summary>
/// Holds parameters and settings for scene cameras.
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

        const glm::mat4x4 &ViewMatrix();
        const glm::mat4x4 &ProjectionMatrix();

        bool ParametersChanged() const;
        /// <summary>
        /// Checks if the bounding volume is inside the
        /// camera frustum <see cref="frustum">
        /// </summary>
        /// <param name="volume">The volume.</param>
        /// <returns></returns>
        bool InFrustum(const BoundingBox &volume);
    private:
        float clipPlaneFar;
        float clipPlaneNear;
        float horizontalFoV;
        float aspectRatio;

        void ComputeViewMatrix();
        void ComputeProjectionMatrix();

        bool projectionChanged;
        bool frustumValuesChanged;

        glm::mat4x4 viewMatrix;
        glm::mat4x4 projectionMatrix;
        CullingFrustum frustum;
};

