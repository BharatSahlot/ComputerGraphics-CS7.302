#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform float fillAmount;
uniform vec3 col;
uniform float border;

void main()
{
    float a = 1 - smoothstep(fillAmount, fillAmount + border, TexCoords.x);
    FragColor = vec4(col, a);
} 
