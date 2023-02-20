#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 cameraRight;
uniform vec3 cameraUp;

uniform float size;
uniform vec3 pos;

void main()
{
    vec3 vertexPos = pos + cameraRight * aPos.x * size + cameraUp * aPos.y * size;

    gl_Position = proj * view * vec4(vertexPos, 1.0);

    TexCoords = aUv;
}
