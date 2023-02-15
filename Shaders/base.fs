#version 330 core

layout (location = 0) out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;

uniform vec3 col;
uniform sampler2D texture1;

void main()
{
    float br = clamp(dot(Normal, normalize(vec3(-0.5, 1, 0))), 0.1, 1) * 3;
    vec3 tex = texture(texture1, TexCoords).rgb * br;
    FragColor = vec4(tex, 1);
}
