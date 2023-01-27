#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).xyz;
    col += texture(bloomTexture, TexCoords).xyz;
    // col = vec3(1) - exp(-col * 1.f);
    // col = pow(col, vec3(1.0 / 1.3f));
    FragColor = vec4(col, 1.f);
    // FragColor = texture(bloomTexture, TexCoords);
    // FragColor = texture(screenTexture, TexCoords);
}
