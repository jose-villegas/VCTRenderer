#pragma once

#include "../types/scene_object.h"
#include "../types/instance_pool.h"

#include <glm/detail/type_vec3.hpp>
#include <vector>

/// <summary>
/// Holds the parameters that describe a scene light source.
/// Supports for three different types of light sources: point,
/// directional and spot.
/// </summary>
/// <seealso cref="SceneObject" />
class Light : public SceneObject, public InstancePool<Light>
{
    public:
        /// <summary>
        /// Describes the light falloff behavior. Meaning its intensity
        /// depending on the distance of the lighten fragment and the
        /// light source. This being
        /// <code>
        /// f = 1 / (1 + c + (l * d) + (q * d * d))
        /// </code>
        /// where f is the falloff, c the <see cref="Constant"/> value,
        /// l the <see cref="Linear"/> value, q the <see cref="Quadratic"/>
        /// value and d the distance.
        /// </summary>
        class Attenuation
        {
            public:
                /// <summary>
                /// The linear falloff factor or l
                /// </summary>
                /// <param name="val">The value.</param>
                void Linear(const float &val) { linear = val; }
                /// <summary>
                /// The quadratic falloff factor or q
                /// </summary>
                /// <param name="val">The value.</param>
                void Quadratic(const float &val) { quadratic = val; }
                /// <summary>
                /// The constant falloff value or c
                /// </summary>
                /// <param name="val">The value.</param>
                void Constant(const float &val) { constant = val; }

                const float &Linear() const { return linear; }
                const float &Quadratic() const { return quadratic; }
                const float &Constant() const { return constant; }

                Attenuation() : constant(1.0f), linear(0.2f), quadratic(0.08f) {};
                ~Attenuation() {};
            private:
                float constant;
                float linear;
                float quadratic;
        };
        enum LightType
        {
            Directional,
            Point,
            Spot
        };

        static const unsigned int DirectionalsLimit;
        static const unsigned int PointsLimit;
        static const unsigned int SpotsLimit;

        /// <summary>
        /// For a spot light the angle of the inner circle
        /// given in radians
        /// </summary>
        /// <param name="val">The value.</param>
        void AngleInnerCone(const float &val);
        /// <summary>
        /// For a spot light the angle of the outer cone or
        /// basically the extent of the light, give in radians.
        /// </summary>
        /// <param name="val">The value.</param>
        void AngleOuterCone(const float &val);
        /// <summary>
        /// Light ambient color component.
        /// </summary>
        /// <param name="val">The value.</param>
        void Ambient(const glm::vec3 &val);
        /// <summary>
        /// Light diffuse color component.
        /// </summary>
        /// <param name="val">The value.</param>
        void Diffuse(const glm::vec3 &val);
        /// <summary>
        /// Light specular color component.
        /// </summary>
        /// <param name="val">The value.</param>
        void Specular(const glm::vec3 &val);
        /// <summary>
        /// Intensities of the ambient, diffuse and specular
        /// components as x, y, and z respectively
        /// </summary>
        /// <param name="val">The value.</param>
        void Intensities(const glm::vec3 &val);

        float AngleInnerCone() const;
        float AngleOuterCone() const;
        const glm::vec3 &Ambient() const;
        const glm::vec3 &Diffuse() const;
        const glm::vec3 &Specular() const;
        const glm::vec3 &Intensities() const;
        const glm::vec3 &Direction() const;
        /// <summary>
        /// The attenuation structure describes the attenuation falloff
        /// function for spot and point lights
        /// </summary>
        Attenuation attenuation;
        /// <summary>
        /// Returns the light's type, directional, point or spot
        /// </summary>
        /// <returns></returns>
        LightType Type() const;
        /// <summary>
        /// Changes the light type and also adds it to one of of the
        /// light static vectors storing lights references per type of light.
        /// </summary>
        /// <param name="val">The value.</param>
        /// <param name="force">
        /// if set to <c>true</c> it will add the light to
        /// a collection vector without checking for previous addition.
        /// This is useful if <see cref="ResetCollections"/> has been called
        /// previously and there is a need to re-add light's to their type
        /// static collection.
        /// </param>
        void TypeCollection(LightType val);
        /// <summary>
        /// Cleans the light type static collection vectors.
        /// </summary>
        static void ResetCollections();
        /// <summary>
        /// Returns the collection of directional lights
        /// </summary>
        /// <returns></returns>
        static const std::vector<Light *> &Directionals();
        /// <summary>
        /// Returns the collection of point lights
        /// </summary>
        /// <returns></returns>
        static const std::vector<Light *> &Points();
        /// <summary>
        /// Returns the collection of spot lights
        /// </summary>
        /// <returns></returns>
        static const std::vector<Light *> &Spots();

        Light();
        virtual ~Light();
    private:
        static std::vector<Light *> directionals;
        static std::vector<Light *> points;
        static std::vector<Light *> spots;
        int collectionIndex;
        float angleInnerCone;
        float angleOuterCone;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        /// <summary>
        /// The intensity per component
        /// x * ambient, y * diffuse, z * specular
        /// </summary>
        glm::vec3 intensity;
        LightType lightType;
};

