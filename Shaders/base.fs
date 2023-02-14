#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 Normal;
in vec2 TexCoords;

uniform vec3 col;
uniform sampler2D texture1;

void main()
{
    float br = clamp(dot(Normal, normalize(vec3(-0.5, 1, 0))), 0.1, 1) * 3;
    vec3 tex = texture(texture1, TexCoords).rgb * br;
    FragColor = vec4(tex, 1);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) BrightColor = FragColor;
    else BrightColor = vec4(0);
}
