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

uniform vec3 col;

void main()
{
    vec2 uv = vec2((index % texXCount) * spriteDims.x / texDims.x, (texYCount - (index / texXCount)) * spriteDims.y / texDims.y);

    uv += vec2(Uv.x * spriteDims.x / texDims.x, Uv.y * spriteDims.y / texDims.y);

    vec4 tex = texture(texture1, uv);
    // FragColor = mix(vec4(0, 0, 1, 1), vec4(0.7, 0.7, 1, 1), tex.r) * tex;
    FragColor = mix(vec4(0, 0, 1, 1), vec4(col, 1), tex.r) * tex;
    // FragColor = vec4(col, 1);

    if(FragColor.r < 0.1) FragColor.a = 0;
    BrightColor = FragColor;
}
