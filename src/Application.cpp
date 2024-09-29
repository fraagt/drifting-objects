#include <cstdlib>
#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include "Application.hpp"
#include "tiny_obj_loader.h"
#include "utils/files.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "glfwxx/KeyCode.hpp"
#include "glfwxx/BufferUsage.hpp"

typedef struct {
    glm::vec3 position;
    glm::vec3 color;
} InstanceData;


int size = 75;  // Grid dimension, so the total number of cubes is gridSize^2
float objectSize = 1.0f;
float interval = 0.5f;

int objectCount = size * size * size;

// Compute the total span for the grid to center it at 0
float span = static_cast<float>(size - 1) * (objectSize + interval);
float startOffset = -span / 2.0f;

std::unique_ptr<Camera> camera;
std::unique_ptr<Mesh> mesh;
std::unique_ptr<Shader> shader;
std::vector<InstanceData> instanceData;

float lastTime = 0.0f;          // Time of the last frame
float lastDelta = 1.0f / 60.0f; // Smoothed delta time
int frameCount = 0;             // Number of frames since the last title update
float fpsTime = 0.0f;           // Time accumulator for FPS calculation
float startTime = 0.0f;         // Record the start time


Application::Application() {
    m_width = 1280;
    m_height = 720;
    m_title = "SpinningSpheres";

    std::srand(std::time(nullptr));

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
    glfwSwapInterval(1);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                InstanceData objectInstanceData;

                // Calculate the position
                objectInstanceData.position.x = startOffset + static_cast<float>(k) * (objectSize + interval);
                objectInstanceData.position.y = startOffset + static_cast<float>(i) * (objectSize + interval);
                objectInstanceData.position.z = startOffset + static_cast<float>(j) * (objectSize + interval);

                objectInstanceData.color.r = static_cast<float>(rand() % 256) / 255.0f;
                objectInstanceData.color.g = static_cast<float>(rand() % 256) / 255.0f;
                objectInstanceData.color.b = static_cast<float>(rand() % 256) / 255.0f;

                instanceData.emplace_back(objectInstanceData);
            }
        }
    }

    camera = std::make_unique<Camera>();

    camera->transform.position.x -= 25.0f;
    camera->fov = 45.0f;
    camera->m_plane = glm::vec2(m_width, m_height);
    camera->m_zNear = 0.1f;
    camera->m_zFar = 1000.0f;


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

    mesh = std::make_unique<Mesh>(vertices, indices);

    std::vector<AttributeLayout> layout = {
            {2, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), offsetof(InstanceData, position), 1},
            {3, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), offsetof(InstanceData, color),    1}
    };

    mesh->addInstanceBuffer(layout,
                            objectCount * sizeof(InstanceData),
                            instanceData.data(),
                            glfwxx::BufferUsage::StaticDraw);

    std::string vertexShaderCode = utils::readFile("assets\\shaders\\instanced.vert");
    std::string fragmentShaderCode = utils::readFile("assets\\shaders\\instanced.frag");

    shader = std::make_unique<Shader>(vertexShaderCode.c_str(), fragmentShaderCode.c_str());

    glViewport(0, 0, m_width, m_height);
}

void Application::run() {
    startTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(m_pWindow)) {
        auto frameStartTime = std::chrono::high_resolution_clock::now(); // Start time of the frame

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
        auto cameraMoveSpeed = 25.0f;
        auto cameraRotateSpeed = 25.0f;
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::A)) {
            camera->transform.position.z -= cameraMoveSpeed * delta;
        }
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::D)) {
            camera->transform.position.z += cameraMoveSpeed * delta;
        }
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::W)) {
            camera->transform.position.x += cameraMoveSpeed * delta;
        }
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::S)) {
            camera->transform.position.x -= cameraMoveSpeed * delta;
        }
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::Q)) {
            camera->transform.position.y += cameraMoveSpeed * delta;
        }
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::E)) {
            camera->transform.position.y -= cameraMoveSpeed * delta;
        }
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::Left)) {
            camera->transform.rotation.y -= cameraRotateSpeed * delta;
        }
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::Right)) {
            camera->transform.rotation.y += cameraRotateSpeed * delta;
        }
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::Up)) {
            camera->transform.rotation.x += cameraRotateSpeed * delta;
        }
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::Down)) {
            camera->transform.rotation.x -= cameraRotateSpeed * delta;
        }
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::LeftShift)) {
            cameraMoveSpeed *= 1.15f;
        }
        if (glfwGetKey(m_pWindow, glfwxx::KeyCode::LeftControl)) {
            cameraMoveSpeed /= 1.15f;
        }

        glfwxx::KeyCode val = glfwxx::KeyCode::G;
        val.value();

        glClearColor(0.10f, 0.15f, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();

//        for (int i = 0; i < objectCount; ++i) {
//            auto model = glm::translate(glm::identity<glm::mat4>(), instanceData[i].position);
//            auto mvp = camera->getProjection() * camera->getView() * model;
//
//            shader->setMatrix4fv("transform", mvp);
//            mesh->draw();
//        }

        auto mvp = camera->getProjection() * camera->getView() * glm::identity<glm::mat4>();

        shader->use();
        shader->setMatrix4fv("transform", mvp);

        mesh->drawInstanced(objectCount);

        glfwSwapBuffers(m_pWindow);
    }
}
