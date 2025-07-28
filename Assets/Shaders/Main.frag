#version 330 core

// Constants
#define RM_DEFAULT 0
#define RM_WIREFRAME 1

#define MAX_DIR_LIGHTS 4
#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS 8

// Structs
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

// Input
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in mat3 TBN;

// Output
out vec4 FragColor;

// Uniforms
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

// Functions
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

    // Base color with alpha check
    vec4 texColor = u_Material.hasDiffuseTexture ? 
                   texture(u_Material.diffuseTexture, TexCoord) : 
                   vec4(u_Material.diffuseColor, 1.0);
    
    // Get specular color from texture if available
    vec3 specularColor = u_Material.hasSpecularTexture ?
                        texture(u_Material.specularTexture, TexCoord).rgb :
                        u_Material.specularColor;
    
    // Get opacity from texture if available
    float opacity = u_Material.hasOpacityTexture ?
                   texture(u_Material.opacityTexture, TexCoord).r :
                   texColor.a;
    
    // Alpha test - discard fully transparent fragments
    if (opacity < 0.01) {
        discard;
    }
    
    vec3 diffuseColor = texColor.rgb;
    
    // Normals and view direction
    vec3 norm;
    if (u_Material.hasNormalTexture) {
        norm = CalculateNormalFromMap();
    } else {
        norm = normalize(Normal);
    }

    vec3 viewDir = normalize(u_CameraPos - FragPos);
    
    // Result color starts with ambient
    vec3 result = u_Material.ambientColor * diffuseColor;
    
    // Directional lights
    for(int i = 0; i < u_DirLightCount; i++)
        result += CalculateDirectionalLight(u_DirLights[i], norm, viewDir, diffuseColor, specularColor);
    
    // Point lights
    for(int i = 0; i < u_PointLightCount; i++)
        result += CalculatePointLight(u_PointLights[i], norm, FragPos, viewDir, diffuseColor, specularColor);
    
    // Spot lights
    for(int i = 0; i < u_SpotLightCount; i++)
        result += CalculateSpotLight(u_SpotLights[i], norm, FragPos, viewDir, diffuseColor, specularColor);
    
    // Add emissive light if texture is available
    if (u_Material.hasEmissiveTexture) {
        vec3 emissive = texture(u_Material.emissiveTexture, TexCoord).rgb;
        result += emissive;
    }
    
    // Use computed opacity (from texture or diffuse alpha)
    FragColor = vec4(result, opacity);
}

vec3 CalculateNormalFromMap()
{
    // Sample normal from the normal map in [0, 1] range
    vec3 normalTexture = texture(u_Material.normalTexture, TexCoord).rgb;

    // Transform to [-1, 1] range
    normalTexture = normalTexture * 2.0 - 1.0;

    // Transform from tangent space to world space
    return normalize(TBN * normalTexture);
}

// Calculates directional light contribution
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
    // Ambient
    vec3 ambient = light.color * light.intensity * u_Material.ambientColor * diffuseColor;
    
    // Diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * light.intensity * diff * diffuseColor;
    
    // Specular (Phong)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = light.color * light.intensity * spec * specularColor;
    
    return ambient + diffuse + specular;
}

// Calculates point light contribution
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
    // Light direction and distance
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // Ambient
    vec3 ambient = light.color * light.intensity * u_Material.ambientColor * diffuseColor;
    
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * light.intensity * diff * diffuseColor;
    
    // Specular (Phong)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = light.color * light.intensity * spec * specularColor;
    
    // Apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return ambient + diffuse + specular;
}

// Calculates spot light contribution
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
    // Light direction and distance
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // Spot light intensity (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    
    // Ambient (not affected by spot direction)
    vec3 ambient = light.color * light.intensity * u_Material.ambientColor * diffuseColor;
    
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * light.intensity * diff * diffuseColor;
    
    // Specular (Phong)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = light.color * light.intensity * spec * specularColor;
    
    // Apply attenuation and spot intensity
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return ambient + diffuse + specular;
}