#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 oCol;

void main()
{
    FragColor = vec4(oCol, 1.0f);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) BrightColor = FragColor;
    else BrightColor = vec4(0);
}
