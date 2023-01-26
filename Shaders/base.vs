#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec3 oCol;
out vec2 Uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 col;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    oCol = col;
    Uv = aUv;
}
