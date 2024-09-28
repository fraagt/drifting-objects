#pragma once

#include <memory>
#include "utils/Common.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

class Application {
public:
    Application();

    void run();

private:
    GLFWwindow *m_pWindow;
    int m_width;
    int m_height;
    std::string m_title;

    std::unique_ptr<Camera> m_pCamera;
    std::unique_ptr<Mesh> m_pMesh;
    std::unique_ptr<Shader> m_pShader;

    std::vector<Transform> m_transforms;
    std::vector<float> m_rotationSpeeds;
    std::vector<float> m_moveSpeeds;
    std::vector<glm::vec3> m_rotateDirections;
    std::vector<glm::vec3> m_velocities;
};
