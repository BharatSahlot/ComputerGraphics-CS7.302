#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 Uv;

uniform sampler2D texture1;

void main()
{
    // FragColor = vec4(Uv.x, Uv.y, 0.f, 1.f);
    FragColor = texture(texture1, Uv);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0) BrightColor = FragColor;
    else BrightColor = vec4(0);
}
