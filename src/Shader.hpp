#pragma once

#include "utils/Common.hpp"
#include <string>

class Shader {
public:
    Shader(const char *vertexShaderCode, const char *fragmentShaderCode);

    ~Shader();

    GLuint getID() const;

    void use() const;

    void setMatrix4fv(const char *location, const glm::mat4 &matrix) const;

    void setVertexAttribPointer(const char *location);

    void setInt(const char *location, GLint value);

    void setFloat(const char *location, GLfloat value);

    void setVec4f(const char *location, glm::vec4 value);

    void setVec3f(const char *location, glm::vec3 value);


private:
    static void checkCompileErrors(unsigned int shader, const std::string &type);

private:
    GLuint m_ID;
};