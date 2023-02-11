#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;
in vec3 Normal;

uniform vec3 col;
uniform sampler2D texture1;

void main()
{
    float br = clamp(dot(Normal, normalize(vec3(-0.5, 1, 0))), 0.1, 1);
    vec3 tex = vec3(0.2, 0.2, 0.2) * br;
    FragColor = vec4(tex, 1);
    BrightColor = vec4(0, 0, 0, 1);
}
