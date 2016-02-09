#pragma once

#include <glm/mat4x4.hpp>
#include <glm/detail/type_vec3.hpp>
#include <array>
#include <memory>

class BoundingVolume
{
    public:
        void MinPoint(const glm::vec3 &val);
        const glm::vec3 &MinPoint(bool transformed = false) const;
        /// <summary>
        /// Sets the minimum point only if <see cref="value">
        /// ifs smaller than the current <see cref="minPoint">.
        /// Sets <see cref="center"> and <see cref="extent">.
        /// </summary>
        /// <param name="val">The value.</param>
        void MaxPoint(const glm::vec3 &val);
        const glm::vec3 &MaxPoint(bool transformed = false) const;
        /// <summary>
        /// Sets the maximum point only if <see cref="value">
        /// ifs bigger than the current <see cref="maxPoint">.
        /// Sets <see cref="center"> and <see cref="extent">.
        /// </summary>
        /// <param name="val">The value.</param>

        void Reset();
        void Transform(const glm::mat4x4 &model);

        BoundingVolume();
        BoundingVolume(const glm::vec3 &min, const glm::vec3 &max);
        virtual ~BoundingVolume();
    protected:
        virtual void BoundariesChanged() {};
    private:
        std::array<glm::vec3, 2> boundaries;
        std::shared_ptr<BoundingVolume> original;
};

class BoundingSphere : public BoundingVolume
{
    public:
        float Radius() const;
        const glm::vec3 &Center() const;

        explicit BoundingSphere(const BoundingVolume &volume);
        ~BoundingSphere() override;
    protected:
        void BoundariesChanged() override;
    private:
        float radius;
        glm::vec3 center;
};
