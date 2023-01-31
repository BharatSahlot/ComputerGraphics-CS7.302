#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform float fade;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).xyz;
    col += texture(bloomTexture, TexCoords).xyz;
    col = pow(col, vec3(1.0 / 2.2f));
    FragColor = mix(vec4(0, 0, 0, 1), vec4(col, 1.f), fade);
    // FragColor = texture(bloomTexture, TexCoords);
    // FragColor = texture(screenTexture, TexCoords);
}
