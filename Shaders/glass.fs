#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform vec3 col;

void main()
{
    FragColor = vec4(1, 1, 1, 0.2);
    // FragColor = vec4(texture(texture1, TexCoords).rgb, 1.0f);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) BrightColor = FragColor;
    else BrightColor = vec4(0);
}
