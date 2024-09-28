#include "Shader.hpp"
#include "utils/not_defined.hpp"
#include <iostream>

Shader::Shader(const char *vertexShaderCode, const char *fragmentShaderCode) {
    GLuint vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    glLinkProgram(m_ID);
    checkCompileErrors(m_ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(m_ID);
}

GLuint Shader::getID() const {
    return m_ID;
}

void Shader::use() const {
    glUseProgram(m_ID);
}

void Shader::checkCompileErrors(unsigned int shader, const std::string &type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << '\n';
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << '\n';
        }
    }
}

void Shader::setInt(const char *location, GLint value) {
    throw exceptions::not_defined();
}

void Shader::setMatrix4fv(const char *location, const glm::mat4 &matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, location), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVertexAttribPointer(const char *location) {
    throw exceptions::not_defined();
}

void Shader::setFloat(const char *location, GLfloat value) {
    glUniform1f(glGetUniformLocation(m_ID, location), value);
}

void Shader::setVec4f(const char *location, glm::vec4 value) {
    glUniform4f(glGetUniformLocation(m_ID, location), value.x, value.y, value.z, value.w);
}

void Shader::setVec3f(const char *location, glm::vec3 value) {
    glUniform3f(glGetUniformLocation(m_ID, location), value.x, value.y, value.z);
}
