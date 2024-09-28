#include "Utils.hpp"
#include "vector_utils.hpp"
#include <array>
#include <functional>

namespace utils {

    glm::mat4 rotateXYZ(glm::mat4 &matrix, const glm::vec3 &angles) {
        matrix = glm::rotate(matrix, angles.z, utils::FRONT);
        matrix = glm::rotate(matrix, angles.y, utils::UP);
        matrix = glm::rotate(matrix, angles.x, utils::RIGHT);
        return matrix;
    }

    glm::mat4 rotateXZY(glm::mat4 &matrix, const glm::vec3 &angles) {
        matrix = glm::rotate(matrix, angles.y, utils::UP);
        matrix = glm::rotate(matrix, angles.z, utils::FRONT);
        matrix = glm::rotate(matrix, angles.x, utils::RIGHT);
        return matrix;
    }

    glm::mat4 rotateYXZ(glm::mat4 &matrix, const glm::vec3 &angles) {
        matrix = glm::rotate(matrix, angles.z, utils::FRONT);
        matrix = glm::rotate(matrix, angles.x, utils::RIGHT);
        matrix = glm::rotate(matrix, angles.y, utils::UP);
        return matrix;
    }

    glm::mat4 rotateYZX(glm::mat4 &matrix, const glm::vec3 &angles) {
        matrix = glm::rotate(matrix, angles.x, utils::RIGHT);
        matrix = glm::rotate(matrix, angles.z, utils::FRONT);
        matrix = glm::rotate(matrix, angles.y, utils::UP);
        return matrix;
    }

    glm::mat4 rotateZXY(glm::mat4 &matrix, const glm::vec3 &angles) {
        matrix = glm::rotate(matrix, angles.y, utils::UP);
        matrix = glm::rotate(matrix, angles.x, utils::RIGHT);
        matrix = glm::rotate(matrix, angles.z, utils::FRONT);
        return matrix;
    }

    glm::mat4 rotateZYX(glm::mat4 &matrix, const glm::vec3 &angles) {
        matrix = glm::rotate(matrix, angles.x, utils::RIGHT);
        matrix = glm::rotate(matrix, angles.y, utils::UP);
        matrix = glm::rotate(matrix, angles.z, utils::FRONT);
        return matrix;
    }

    using rotation_func = std::function<glm::mat4(glm::mat4 &, const glm::vec3 &)>;

    const std::array<rotation_func, 6> rotation_funcs = {
            rotateXYZ,
            rotateXZY,
            rotateYXZ,
            rotateYZX,
            rotateZXY,
            rotateZYX
    };

    glm::mat4 getMatrix(glm::vec3 position, glm::vec3 rotationEuler, glm::vec3 scale, RotationOrder order) {
        auto matrix = glm::identity<glm::mat4>();

        matrix = glm::translate(matrix, position);
        matrix = rotation_funcs[static_cast<size_t>(order)](matrix, glm::radians(rotationEuler));
        matrix = glm::scale(matrix, scale);

        return matrix;
    }

    glm::vec3 lookAt(glm::vec3 origin, glm::vec3 target, glm::vec3 up) {
        auto direction = glm::normalize(target - origin);

        float yaw = glm::atan(direction.z, direction.x);
        float pitch = glm::atan(direction.y);
        float roll = 0.0f;

        yaw = glm::degrees(yaw);
        pitch = glm::atan(pitch);

        return {pitch, yaw, roll};
    }

    glm::vec3 getDirection(const glm::vec3 &radianRotation) {
        const auto yaw_cos = glm::cos(radianRotation.y);
        const auto yaw_sin = glm::sin(radianRotation.y);
        const auto pitch_cos = glm::cos(radianRotation.x);
        const auto pitch_sin = glm::sin(radianRotation.x);

        return glm::normalize(glm::vec3(yaw_cos * pitch_cos, pitch_sin, yaw_sin * pitch_cos));
    }
}