#pragma once

#include <array>
#include <memory>
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>

class BoundingBox
{
    public:
        /// <summary>
        /// Sets the minimum point only if <see cref="value">
        /// ifs smaller than the current <see cref="minPoint">.
        /// Sets <see cref="center"> and <see cref="extent">.
        /// </summary>
        /// <param name="val">The value.</param>
        void MinPoint(const glm::vec3 &val);
        const glm::vec3 &MinPoint(bool transformed = false) const;
        /// <summary>
        /// Sets the maximum point only if <see cref="value">
        /// ifs bigger than the current <see cref="maxPoint">.
        /// Sets <see cref="center"> and <see cref="extent">.
        /// </summary>
        /// <param name="val">The value.</param>
        void MaxPoint(const glm::vec3 &val);
        const glm::vec3 &MaxPoint(bool transformed = false) const;

        const glm::vec3 &Center(bool transformed = false) const;
        const glm::vec3 &Extent(bool transformed = false) const;

        void Reset();
        void Transform(const glm::mat4x4 &model);

        BoundingBox();
        virtual ~BoundingBox();
    private:
        std::array<glm::vec3, 2> boundaries;
        std::shared_ptr<BoundingBox> original;

        glm::vec3 center;
        glm::vec3 extent;
};
