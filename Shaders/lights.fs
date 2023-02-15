#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 col;
uniform sampler2D texture1;

void main()
{
    vec3 tex = vec3(1, 0.8, 0);
    FragColor = vec4(tex, 1);
}
