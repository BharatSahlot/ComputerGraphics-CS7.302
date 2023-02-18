#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform vec3 col;

void main()
{
    float br = pow(1 - smoothstep(0, 1.4, distance(TexCoords, vec2(0.5, 0))), 5);
    br = 1 - smoothstep(0.8, 0.85, 1 - br);
    br *= 0.5;
    FragColor = vec4(0.1, 0.9, 0.9, br);
}
