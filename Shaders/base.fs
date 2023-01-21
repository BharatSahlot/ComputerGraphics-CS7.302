#version 330 core

in vec3 oNormal;
in vec3 oCol;

out vec4 FragColor;

void main()
{
    float diff = (2.0 + dot(oNormal, normalize(vec3(1, 1, 1)))) / 2.0;
    
    FragColor = vec4(oCol, 1);
    // FragColor = vec4(normalize(vec3(2, 2, 2) + oNormal), 1.0f) * diff;
    // FragColor = vec4(oNormal.x, 0.25f, oNormal.z, 1.0f) * diff;
}
