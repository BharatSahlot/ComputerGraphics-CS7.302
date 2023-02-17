#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform vec3 col;

void main()
{
    FragColor = vec4(0, 0, 0, 1);
    discard;
}
