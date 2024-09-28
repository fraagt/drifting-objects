#pragma once

#include "utils/Common.hpp"
#include "utils/Utils.hpp"
#include "utils/vector_utils.hpp"

struct Transform {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    Transform() = default;

    Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
            : position(position), rotation(rotation), scale(scale) {}


    inline glm::mat4 getMatrix() const {
        return utils::getMatrix(position, rotation, scale, utils::RotationOrder::ZYX);
    }

    inline glm::vec3 getFront() const {
        return utils::getDirectionEuler(rotation);
    }

    inline glm::vec3 getRight() const {
        return glm::cross(getFront(), utils::UP);
    }

    inline glm::vec3 getUp() const {
        return glm::cross(getRight(), getFront());
    }
};