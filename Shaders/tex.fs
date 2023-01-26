#version 330 core

in vec2 Uv;
in vec3 oCol;

out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    // FragColor = vec4(Uv.x, Uv.y, 0.f, 1.f);
    FragColor = texture(texture1, Uv);
}
