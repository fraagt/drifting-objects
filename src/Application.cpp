#include <cstdlib>
#include <iostream>
#include <ctime>
#include "Application.hpp"
#include "tiny_obj_loader.h"
#include "utils/files.hpp"

int size = 10;  // Grid dimension, so the total number of cubes is gridSize^2
float cubeSize = 1.0f;
float interval = 0.5f;

// Compute the total span for the grid to center it at 0
float spanY = static_cast<float>(size - 1) * (cubeSize + interval);
float spanZ = static_cast<float>(size - 1) * (cubeSize + interval);

// Start positions to center the grid
float startPosY = -spanY / 2.0f;
float startPosZ = -spanZ / 2.0f;

float moveSpeed = 1.0f;
float moveSpeedDeltaRatio = 0.4f;
float moveSpeedDelta = moveSpeed * moveSpeedDeltaRatio;

float maxSpeed = moveSpeed * (1.0f + moveSpeedDeltaRatio);

float rotationSpeed = 90.0f;
float rotationSpeedDeltaRatio = 0.6f;
float rotationSpeedDelta = rotationSpeed * rotationSpeedDeltaRatio;


float lastTime = 0.0f;          // Time of the last frame
float lastDelta = 1.0f / 60.0f; // Smoothed delta time
int frameCount = 0;             // Number of frames since the last title update
float fpsTime = 0.0f;           // Time accumulator for FPS calculation
float startTime = 0.0f;         // Record the start time


Application::Application() {
    m_width = 1280;
    m_height = 720;
    m_title = "SpinningSpheres";

//    std::srand(std::time(nullptr));

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
    if (m_pWindow == nullptr) {
        std::cout << "Failed to create GLFW m_window" << '\n';
        glfwTerminate();
        throw EXIT_FAILURE;
    }

    glfwSetWindowUserPointer(m_pWindow, reinterpret_cast<void *>(this));
    glfwMakeContextCurrent(m_pWindow);

    if (!gladLoaderLoadGL()) {
        std::cout << "Failed to load GLAD" << '\n';
        throw EXIT_FAILURE;
    }

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glfwSwapInterval(0);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Transform transform;

            // Set initial position for the cube, centered at (0, 0, 0)
            transform.position.y = startPosY + static_cast<float>(i) * (cubeSize + interval);
            transform.position.z = startPosZ + static_cast<float>(j) * (cubeSize + interval);


            auto objectMoveSpeed = moveSpeed - (moveSpeedDelta / 2.0f) +
                                   (static_cast<float>(std::rand() % 1001) / 1000.0f) * moveSpeedDelta;


            auto objectRotationSpeed = rotationSpeed - (rotationSpeedDelta / 2.0f) +
                                       (static_cast<float>(std::rand() % 1001) / 1000.0f) * rotationSpeedDelta;

            glm::vec3 rotationDirection{
                    (static_cast<float>(std::rand() % 1001) / 1000.0f) * 2.0f - 1.0f,
                    (static_cast<float>(std::rand() % 1001) / 1000.0f) * 2.0f - 1.0f,
                    (static_cast<float>(std::rand() % 1001) / 1000.0f) * 2.0f - 1.0f};

            glm::vec3 velocity{
                    (static_cast<float>(std::rand() % 1001) / 1000.0f) * 2.0f - 1.0f,
                    (static_cast<float>(std::rand() % 1001) / 1000.0f) * 2.0f - 1.0f,
                    (static_cast<float>(std::rand() % 1001) / 1000.0f) * 2.0f - 1.0f
            };

            m_moveSpeeds.emplace_back(objectMoveSpeed);
            m_rotationSpeeds.emplace_back(objectRotationSpeed);
            m_transforms.emplace_back(transform);
            m_rotateDirections.emplace_back(rotationDirection);
            m_velocities.emplace_back(velocity);
        }
    }

    m_pCamera = std::make_unique<Camera>();

    m_pCamera->transform.position.x -= 25.0f;
    m_pCamera->fov = 45.0f;
    m_pCamera->m_plane = glm::vec2(m_width, m_height);
    m_pCamera->m_zNear = 0.1f;
    m_pCamera->m_zFar = 100.0f;


    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string error;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, "assets\\models\\cube.obj")) {
        std::cout << error << std::endl;
    }

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    for (GLuint i = 0; i < shapes[0].mesh.indices.size(); ++i) {
        const auto &index = shapes[0].mesh.indices[i];

        vertices.push_back({{
                                    attrib.vertices[(index.vertex_index * 3)],
                                    attrib.vertices[(index.vertex_index * 3) + 1],
                                    attrib.vertices[(index.vertex_index * 3) + 2],
                            },
                            {
                                    attrib.normals[(index.normal_index * 3)],
                                    attrib.normals[(index.normal_index * 3) + 1],
                                    attrib.normals[(index.normal_index * 3) + 2],
                            }});

        indices.push_back(i);
    }

    m_pMesh = std::make_unique<Mesh>(vertices, indices);

    std::string vertexShaderCode = utils::readFile("assets\\shaders\\default.vert");
    std::string fragmentShaderCode = utils::readFile("assets\\shaders\\default.frag");

    m_pShader = std::make_unique<Shader>(vertexShaderCode.c_str(), fragmentShaderCode.c_str());

    glViewport(0, 0, m_width, m_height);
}

void Application::run() {
    startTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(m_pWindow)) {
        glfwPollEvents();

        auto currentTime = static_cast<float>(glfwGetTime());
        auto delta = currentTime - lastTime;
        lastTime = currentTime;

        // Smooth the delta time
        lastDelta = (lastDelta + delta) / 2;

        // Accumulate time for FPS counter
        fpsTime += delta;
        frameCount++;

        // Update the window title every 1 second (or any desired interval)
        if (fpsTime >= 1.0f) {
            float fps = static_cast<float>(frameCount) / fpsTime;  // Calculate FPS
            float elapsedTime = currentTime - startTime;  // Calculate time since start

            // Update the window title with both FPS and elapsed time
            std::string current_title = m_title + " | FPS: " + std::to_string(static_cast<int>(fps))
                                        + " | Time: " + std::to_string(static_cast<int>(elapsedTime)) + "s";
            glfwSetWindowTitle(m_pWindow, current_title.c_str());

            // Reset counters
            fpsTime = 0.0f;
            frameCount = 0;
        }

        for (int i = 0; i < m_transforms.size(); ++i) {
            auto &transform = m_transforms[i];
            auto &objectMoveSpeed = m_moveSpeeds[i];
            auto &objectRotationSpeed = m_rotationSpeeds[i];
            auto &rotateDirection = m_rotateDirections[i];
            auto &velocity = m_velocities[i];

            auto moveStep = velocity * objectMoveSpeed * delta;
            auto rotationStep = rotateDirection * objectRotationSpeed * delta;

            transform.position += moveStep;
            transform.rotation += rotationStep;

            // Gradually change velocity
            float changeAmount = 0.1f; // Controls how much the velocity changes each frame

            glm::vec3 randomChange{
                    (static_cast<float>(std::rand() % 1001) / 1000.0f) * 2.0f - 1.0f,  // Random perturbation in X
                    (static_cast<float>(std::rand() % 1001) / 1000.0f) * 2.0f - 1.0f,  // Random perturbation in Y
                    (static_cast<float>(std::rand() % 1001) / 1000.0f) * 2.0f - 1.0f   // Random perturbation in Z
            };

            velocity += randomChange * changeAmount;  // Add small random change to velocity

            // Clamp the velocity to a reasonable magnitude to avoid extreme speeds
            float speed = glm::length(velocity);
        }


        glClearColor(0.10f, 0.15f, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_pShader->use();

        for (const auto &trans: m_transforms) {
            auto mvp = m_pCamera->getProjection() * m_pCamera->getView() * trans.getMatrix();

            m_pShader->setMatrix4fv("transform", mvp);

            m_pMesh->draw();
        }

        glfwSwapBuffers(m_pWindow);
    }
}
