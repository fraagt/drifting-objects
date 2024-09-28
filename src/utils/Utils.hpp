#pragma once

#include "Common.hpp"

namespace utils {
    enum class RotationOrder : size_t {
        XYZ = 0x0,
        XZY = 0x1,
        YXZ = 0x2,
        YZX = 0x3,
        ZXY = 0x4,
        ZYX = 0x5
    };

    glm::mat4 getMatrix(glm::vec3 position, glm::vec3 rotationEuler, glm::vec3 scale, RotationOrder order);

    glm::vec3 lookAt(glm::vec3 origin, glm::vec3 target, glm::vec3 up);

    glm::vec3 getDirection(const glm::vec3 &radianRotation);

    inline glm::vec3 getDirectionEuler(const glm::vec3 &eulerRotation) {
        return getDirection(glm::radians(eulerRotation));
    }
}