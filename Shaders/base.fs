#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform vec3 col;
uniform sampler2D texture1;

void main()
{
    vec3 tex = texture(texture1, TexCoords).rgb;
    // tex = vec3(TexCoords, 0);
    FragColor = vec4(tex, 1);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) BrightColor = FragColor;
    else BrightColor = vec4(0);
}
