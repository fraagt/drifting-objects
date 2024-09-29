#pragma once

#include "utils/Common.hpp"
#include <vector>
#include "Vertex.hpp"
#include "AttributeLayout.hpp"
#include "glfwxx/BufferUsage.hpp"

class Mesh {
public:
    Mesh();

    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

    ~Mesh();

    void draw();

    void drawInstanced(int instanceCount);

    void setupMesh(const std::vector<Vertex> &vertices,
                   const std::vector<unsigned int> &indices);

    void addInstanceBuffer(const std::vector<AttributeLayout> &layout,
                           GLsizei dataSize,
                           const void *data,
                           glfwxx::BufferUsage usage);

private:
    void setupMesh();

    GLenum mode;
    GLuint VAO, VBO, EBO;
    std::vector<GLuint> instanceVBOs;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
