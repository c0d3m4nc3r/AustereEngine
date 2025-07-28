#version 330 core

// Input
in vec3 TexCoord;

// Output
out vec4 FragColor;

// Uniforms
uniform samplerCube u_Cubemap;

// Functions
void main()
{
    FragColor = texture(u_Cubemap, TexCoord);
}
