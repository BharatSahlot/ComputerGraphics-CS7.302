#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

uniform vec3 col;
uniform sampler2D texture1;

float ambient = 0.3;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(0.2, 1, 0.3));
    float br = max(dot(norm, lightDir), 0.0) + ambient;

    vec3 tex = vec3(0.8, 0.02, 0.02) * br;
    FragColor = vec4(tex, 1);
}
