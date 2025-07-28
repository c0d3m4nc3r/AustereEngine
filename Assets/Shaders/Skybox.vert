#version 330 core

// Input
layout (location = 0) in vec3 aPosition;

// Output
out vec3 TexCoord;

// Uniforms
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

// Functions
void main()
{
    TexCoord = aPosition;

    vec4 pos = u_ProjectionMatrix * u_ViewMatrix * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}  