#pragma once

#include "glad/gl.h"

namespace glfwxx {
    class BufferUsage {
    public:
        using EnumType = GLenum;
        enum : EnumType {
            StreamDraw = GL_STREAM_DRAW,
            StreamRead = GL_STREAM_READ,
            StreamCopy = GL_STREAM_COPY,
            StaticDraw = GL_STATIC_DRAW,
            StaticRead = GL_STATIC_READ,
            StaticCopy = GL_STATIC_COPY,
            DynamicDraw = GL_DYNAMIC_DRAW,
            DynamicRead = GL_DYNAMIC_READ,
            DynamicCopy = GL_DYNAMIC_COPY
        };

    private:
        GLenum _value; // Holds the buffer usage enum value

    public:
        // Default constructor
        BufferUsage() = default;

        // Parameterized constructor for initializing with a specific usage value
        constexpr BufferUsage(GLenum value) : _value(value) {}

        // Default copy constructor
        BufferUsage(const BufferUsage &) = default;

        // Assignment operator for copying buffer usage values
        BufferUsage &operator=(const BufferUsage &) = default;

        // Implicit conversion operator to convert BufferUsage to GLenum
        operator GLenum() const {
            return _value;
        }

        // Comparison operators for convenience
        bool operator==(const BufferUsage &other) const {
            return _value == other._value;
        }

        bool operator!=(const BufferUsage &other) const {
            return _value != other._value;
        }
    };
}