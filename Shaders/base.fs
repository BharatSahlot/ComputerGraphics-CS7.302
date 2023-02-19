#version 330 core

layout (location = 0) out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;

uniform vec3 col;
uniform sampler2D texture1;
uniform float oneMinusAlpha;

float ambient = 0.3;

void main()
{
    if(oneMinusAlpha >= 0.5) discard;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(0.2, 1, -0.7));

    float br = max(dot(norm, lightDir), 0.0) + ambient;
    vec3 tex = texture(texture1, TexCoords).rgb * br;
    FragColor = vec4(tex, 1 - oneMinusAlpha);
}
