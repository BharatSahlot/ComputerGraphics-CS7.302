#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 Uv;

uniform sampler2D texture1;

uniform vec2 texDims;
uniform vec2 spriteDims;

uniform int texXCount;
uniform int texYCount;

uniform int index;

void main()
{
    vec2 uv = vec2((index % texXCount) * spriteDims.x / texDims.x, 
    (texYCount - 1 - (index / texXCount)) * spriteDims.y / texDims.y);

    uv += vec2(Uv.x * spriteDims.x / texDims.x, Uv.y * spriteDims.y / texDims.y);

    vec4 tex = texture(texture1, uv);
    FragColor = tex;

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.96) BrightColor = FragColor;
    else BrightColor = vec4(0);
}
