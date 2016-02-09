#pragma once

#include <glm/detail/type_vec3.hpp>

class Light
{
    public:
        class Attenuation
        {
            public:
                float Linear() const { return linear; }
                void Linear(float val) { linear = val; }
                float Quadratic() const { return quadratic; }
                void Quadratic(float val) { quadratic = val; }
                float Constant() const { return constant; }
                void Constant(float val) { constant = val; }

                Attenuation() : constant(1.0f), linear(0.2f), quadratic(0.08f) {};
                ~Attenuation() {};
            private:
                float constant;
                float linear;
                float quadratic;
        };
        enum LightType
        {
            Disabled,
            Directional,
            Point,
            Spot
        };

        float AngleInnerCone() const;
        void AngleInnerCone(float val);
        float AngleOuterCone() const;
        void AngleOuterCone(float val);

        const glm::vec3 &Ambient() const;
        void Ambient(const glm::vec3 &val);
        const glm::vec3 &Diffuse() const;
        void Diffuse(const glm::vec3 &val);
        const glm::vec3 &Specular() const;
        void Specular(const glm::vec3 &val);

        glm::vec3 Position() const;
        void Position(glm::vec3 val);
        glm::vec3 Direction() const;
        void Direction(glm::vec3 val);

        Attenuation attenuation;

        LightType Type() const;
        void Type(LightType val);

        Light();
        virtual ~Light();
    private:
        float angleInnerCone;
        float angleOuterCone;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 position;
        glm::vec3 direction;
        LightType lightType;
};

