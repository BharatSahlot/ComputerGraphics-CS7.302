#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform vec3 col;

void main()
{
    float br = clamp(dot(Normal, normalize(vec3(0.5, 1, 0))), 0.4, 1);
    vec3 tex = vec3(0.1, 1, 0.3) * br;
    FragColor = vec4(tex, 1);
}
