#version 330 core

// Input
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

// Output
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out mat3 TBN;

// Uniforms
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

// Functions
void main()
{
    TexCoord = aTexCoord;
    FragPos = vec3(u_ModelMatrix * vec4(aPosition, 1.0));

    mat3 normalMatrix = mat3(transpose(inverse(u_ModelMatrix)));

    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    vec3 N = normalize(normalMatrix * aNormal);

    TBN = mat3(T, B, N);
    Normal = N;

    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(FragPos, 1.0);
}
