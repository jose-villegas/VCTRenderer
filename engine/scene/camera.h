#pragma once

#include "../util/single_active.h"
#include "../types/frustum.h"
#include "../types/scene_object.h"

#include <glm/detail/type_vec3.hpp>

/// <summary>
/// Holds parameters and settings for scene cameras.
/// </summary>
/// <seealso cref="SingleActive{Camera}" />
class Camera :  public SceneObject, public SingleActive<Camera>
{
    public:
        Camera();
        virtual ~Camera();

        const glm::mat4x4 &ViewMatrix();
        const glm::mat4x4 &ProjectionMatrix();

        /// <summary>
        /// Checks if the bounding volume is inside the
        /// camera frustum <see cref="CullingFrustum"/>
        /// </summary>
        /// <param name="volume">The volume.</param>
        /// <returns></returns>
        bool InFrustum(const BoundingBox &volume);
        /// <summary>
        /// Sets the <see cref="clipPlaneFar"/> value.
        /// Value is in the range [0.01, inf]
        /// </summary>
        /// <param name="val">The value.</param>
        void ClipPlaneFar(float val);
        /// <summary>
        /// Sets the <see cref="clipPlaneNear"/> value.
        /// Value is in the range [0.01, inf]
        /// </summary>
        /// <param name="val">The value.</param>
        void ClipPlaneNear(float val);
        /// <summary>
        /// Sets the <see cref="horizontalFoV"/> value.
        /// Value is in the range [1, 179]
        /// </summary>
        /// <param name="val">The value.</param>
        void HorizontalFoV(float val);
        void AspectRatio(float val);
        void LookAt(const glm::vec3 &val);
        void Position(const glm::vec3 &val);
        void Up(const glm::vec3 &val);
        bool ParametersChanged() const;
    private:
        float clipPlaneFar;
        float clipPlaneNear;
        float horizontalFoV;
        float aspectRatio;
        glm::vec3 lookAt;
        glm::vec3 position;
        glm::vec3 up;

        void ComputeViewMatrix();
        void ComputeProjectionMatrix();

        bool viewValuesChanged;
        bool projectionValuesChanged;
        bool frustumValuesChanged;

        glm::mat4x4 viewMatrix;
        glm::mat4x4 projectionMatrix;
        CullingFrustum frustum;
};

