#version 330 core

layout (location = 0) out vec4 FragColor;

uniform vec3 col;

void main()
{
    FragColor = vec4(col, 1);
}
