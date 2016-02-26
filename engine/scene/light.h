#pragma once

#include "../types/scene_object.h"

#include <glm/detail/type_vec3.hpp>
#include <vector>

class Light : public SceneObject
{
    public:
        class Attenuation
        {
            public:
                void Linear(float val) { linear = val; }
                void Quadratic(float val) { quadratic = val; }
                void Constant(float val) { constant = val; }

                float Linear() const { return linear; }
                float Quadratic() const { return quadratic; }
                float Constant() const { return constant; }

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

        void AngleInnerCone(float val);
        void AngleOuterCone(float val);
        void Ambient(const glm::vec3 &val);
        void Diffuse(const glm::vec3 &val);
        void Specular(const glm::vec3 &val);
        void Intensity(const glm::vec3 &val);

        float AngleInnerCone() const;
        float AngleOuterCone() const;
        const glm::vec3 &Ambient() const;
        const glm::vec3 &Diffuse() const;
        const glm::vec3 &Specular() const;
        const glm::vec3 &Intensity() const;

        const glm::vec3 &Direction(bool viewSpace = false) const;
        const glm::vec3 &Position(bool viewSpace = false) const;

        void UpdateViewRelative(bool position, bool direction);

        Attenuation attenuation;

        LightType Type() const;
        /// <summary>
        /// Changes the light type and also adds it to one of of the
        /// light vectors storing lights references per type of light.
        /// </summary>
        /// <param name="val">The value.</param>
        /// <param name="force">
        /// if set to <c>true</c> it will add the light to
        /// a collection vector without checking for previous addition.
        /// </param>
        void TypeCollection(LightType val, bool force = false);

        Light();
        virtual ~Light();

        static void CleanCollections();
        static const std::vector<Light *> &Directionals();
        static const std::vector<Light *> &Points();
        static const std::vector<Light *> &Spots();
    private:
        static std::vector<Light *> directionals;
        static std::vector<Light *> points;
        static std::vector<Light *> spots;

        int collectionIndex;

        Transform viewRelative;

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

