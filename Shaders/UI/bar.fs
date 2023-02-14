#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform float fillAmount;
uniform vec3 col;

void main()
{
    float a = 1 - smoothstep(fillAmount, fillAmount + 0.05, TexCoords.x);
    // float a = 1 - step(fillAmount, TexCoords.x);
    FragColor = vec4(col * a, 1);
} 
