#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D text;
uniform vec4 textColor;

uniform float shadowDistance;
uniform vec4 shadowColor;

void main()
{
    float ta = texture(text, TexCoords).r * textColor.a;
    float sa = texture(text, TexCoords + vec2(1, -1) * shadowDistance).r;

    FragColor = mix(shadowColor * sa, vec4(textColor.rgb, ta), ta);

    if(FragColor.a <= 0.1) discard;
} 
