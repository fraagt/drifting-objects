#include "Mesh.hpp"

Mesh::Mesh() : VAO{0}, VBO{0}, EBO{0} {}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices)
        : vertices{vertices}, indices{indices}, mode{GL_TRIANGLES} {
    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    for (auto &vbo: instanceVBOs) {
        glDeleteBuffers(1, &vbo);
    }
}

void Mesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, (GLvoid *) 0);
    glBindVertexArray(0);
}

void Mesh::drawInstanced(int instanceCount) {
    glBindVertexArray(VAO);
    glDrawElementsInstanced(mode, indices.size(), GL_UNSIGNED_INT, (GLvoid *) 0, instanceCount);
    glBindVertexArray(0);
}

void Mesh::setupMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) {
    this->vertices = vertices;
    this->indices = indices;
    setupMesh();
}

void Mesh::addInstanceBuffer(const std::vector<AttributeLayout> &layout,
                             GLsizei dataSize,
                             const void *data,
                             glfwxx::BufferUsage usage) {
    GLuint instanceVBO;

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, static_cast<GLenum>(usage));

    glBindVertexArray(VAO);

    for (const auto &attrib: layout) {
        glEnableVertexAttribArray(attrib.index);
        glVertexAttribPointer(attrib.index,
                              attrib.size,
                              attrib.type,
                              attrib.normalized,
                              attrib.stride,
                              (void *) attrib.offset);
        glVertexAttribDivisor(attrib.index, attrib.divisor);
    }

    glBindVertexArray(0);

    instanceVBOs.push_back(instanceVBO);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, normal));

    glBindVertexArray(0);
}
