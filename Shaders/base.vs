#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 oNormal;

uniform mat4 model;
uniform mat4 vp;

void main()
{
    gl_Position = vp * model * vec4(aPos, 1.0);
    oNormal = aNormal;
}
