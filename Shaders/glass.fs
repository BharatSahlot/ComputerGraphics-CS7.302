#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 col;

void main()
{
    FragColor = vec4(1, 1, 1, 0.2);
}
