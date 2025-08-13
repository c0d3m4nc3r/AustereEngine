#include "Resources/Builtin/Shaders.hpp"

namespace AE::Builtin::Shaders
{
    const std::string standardVertex = R"(
        #version 330 core

        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aNormal;
        layout(location = 2) in vec2 aTexCoord;
        layout(location = 3) in vec3 aTangent;
        layout(location = 4) in vec3 aBitangent;

        out vec2 TexCoord;
        out vec3 Normal;
        out vec3 FragPos;
        out mat3 TBN;

        uniform mat4 u_ModelMatrix;
        uniform mat4 u_ViewMatrix;
        uniform mat4 u_ProjectionMatrix;

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
    )";

    const std::string standardFragment = R"(
        #version 330 core

        #define RM_DEFAULT 0
        #define RM_WIREFRAME 1

        #define MAX_DIR_LIGHTS 4
        #define MAX_POINT_LIGHTS 8
        #define MAX_SPOT_LIGHTS 8

        struct Material {
            vec3 ambientColor;
            vec3 diffuseColor;
            vec3 specularColor;
            float shininess;

            sampler2D diffuseTexture;
            bool hasDiffuseTexture;

            sampler2D specularTexture;
            bool hasSpecularTexture;

            sampler2D emissiveTexture;
            bool hasEmissiveTexture;

            sampler2D normalTexture;
            bool hasNormalTexture;

            sampler2D opacityTexture;
            bool hasOpacityTexture;
        };

        struct DirectionalLight {
            vec3 color;
            float intensity;
            vec3 direction;
        };

        struct PointLight {
            vec3 color;
            float intensity;
            vec3 position;
            float constant;
            float linear;
            float quadratic;
        };

        struct SpotLight {
            vec3 color;
            float intensity;
            vec3 position;
            vec3 direction;
            float innerCutoff;
            float outerCutoff;
            float constant;
            float linear;
            float quadratic;
        };

        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoord;
        in mat3 TBN;

        out vec4 FragColor;

        uniform Material u_Material;
        uniform vec3 u_CameraPos;
        uniform int u_RenderMode;

        uniform DirectionalLight u_DirLights[MAX_DIR_LIGHTS];
        uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
        uniform SpotLight u_SpotLights[MAX_SPOT_LIGHTS];
        uniform sampler2DArray u_DirLightShadowMaps;
        uniform int u_DirLightCount;
        uniform int u_PointLightCount;
        uniform int u_SpotLightCount;

        vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);
        vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);
        vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor);
        vec3 CalculateNormalFromMap();

        void main()
        {
            if (u_RenderMode == RM_WIREFRAME)
            {
                FragColor = vec4(0.0, 1.0, 0.0, 1.0);
                return;
            }

            vec4 texColor = u_Material.hasDiffuseTexture ? 
                        texture(u_Material.diffuseTexture, TexCoord) : 
                        vec4(u_Material.diffuseColor, 1.0);
            
            vec3 specularColor = u_Material.hasSpecularTexture ?
                                texture(u_Material.specularTexture, TexCoord).rgb :
                                u_Material.specularColor;
            
            float opacity = u_Material.hasOpacityTexture ?
                        texture(u_Material.opacityTexture, TexCoord).r :
                        texColor.a;
            
            if (opacity < 0.01) {
                discard;
            }
            
            vec3 diffuseColor = texColor.rgb;
            
            vec3 norm;
            if (u_Material.hasNormalTexture) {
                norm = CalculateNormalFromMap();
            } else {
                norm = normalize(Normal);
            }

            vec3 viewDir = normalize(u_CameraPos - FragPos);
            
            vec3 result = u_Material.ambientColor * diffuseColor;
            
            for(int i = 0; i < u_DirLightCount; i++)
                result += CalculateDirectionalLight(u_DirLights[i], norm, viewDir, diffuseColor, specularColor);
            
            for(int i = 0; i < u_PointLightCount; i++)
                result += CalculatePointLight(u_PointLights[i], norm, FragPos, viewDir, diffuseColor, specularColor);
            
            for(int i = 0; i < u_SpotLightCount; i++)
                result += CalculateSpotLight(u_SpotLights[i], norm, FragPos, viewDir, diffuseColor, specularColor);
            
            if (u_Material.hasEmissiveTexture)
            {
                vec3 emissive = texture(u_Material.emissiveTexture, TexCoord).rgb;
                result += emissive;
            }
            
            FragColor = vec4(result, opacity);
        }

        vec3 CalculateNormalFromMap()
        {
            vec3 normalTexture = texture(u_Material.normalTexture, TexCoord).rgb;
            normalTexture = normalTexture * 2.0 - 1.0;
            return normalize(TBN * normalTexture);
        }

        vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
        {
            vec3 ambient = light.color * light.intensity * u_Material.ambientColor * diffuseColor;
            
            vec3 lightDir = normalize(-light.direction);
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = light.color * light.intensity * diff * diffuseColor;
            
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
            vec3 specular = light.color * light.intensity * spec * specularColor;
            
            return ambient + diffuse + specular;
        }

        vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
        {
            vec3 lightDir = normalize(light.position - fragPos);
            float distance = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
            
            vec3 ambient = light.color * light.intensity * u_Material.ambientColor * diffuseColor;
            
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = light.color * light.intensity * diff * diffuseColor;
            
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
            vec3 specular = light.color * light.intensity * spec * specularColor;
            
            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;
            
            return ambient + diffuse + specular;
        }

        vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
        {
            vec3 lightDir = normalize(light.position - fragPos);
            float distance = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
            
            float theta = dot(lightDir, normalize(-light.direction));
            float epsilon = light.innerCutoff - light.outerCutoff;
            float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
            
            vec3 ambient = light.color * light.intensity * u_Material.ambientColor * diffuseColor;
            
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = light.color * light.intensity * diff * diffuseColor;
            
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
            vec3 specular = light.color * light.intensity * spec * specularColor;
            
            diffuse *= attenuation * intensity;
            specular *= attenuation * intensity;
            
            return ambient + diffuse + specular;
        }
    )";
}