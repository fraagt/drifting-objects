#pragma once

#include <vector>
#include <memory>
#include "utils/Common.hpp"
#include "glm/vec3.hpp"
#include "Vertex.hpp"

class Mesh {
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);

    void draw();

private:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;

    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ebo;
    GLenum m_mode;
};
