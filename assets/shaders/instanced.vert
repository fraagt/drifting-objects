#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_instance_position;
layout (location = 3) in vec3 a_instance_color;

uniform mat4 transform;

out vec3 color;

void main() {
    vec3 world_posiion = a_position + a_instance_position;

    gl_Position = transform * vec4(world_posiion, 1.0);

    color = a_instance_color;
}