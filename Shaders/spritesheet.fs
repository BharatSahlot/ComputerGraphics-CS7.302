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

    BrightColor = vec4(0);
}
