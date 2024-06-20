#version 330 core

in vec3 vo_Position;
in vec3 vo_Normal;
in vec2 vo_TexCoords;

out vec4 fo_Colour;

// See C++ class Material
struct Material
{
    sampler2D fu_DiffuseTex;

    vec3 fu_SpecularColour;
    float fu_Shininess;

    float fu_Transparency;
};
uniform Material material;

// See C++ class DirectionalLight
layout (std140) uniform ubo_DirectionalLight
{
    vec4 fu_Direction;

    vec4 fu_AmbientReflectCoef;
    vec4 fu_DiffuseReflectCoef;
    vec4 fu_SpecularReflectCoef;

    bool fu_IsBlinn;
} directionalLight;

// See C++ class PointLight
layout (std140) uniform ubo_PointLight
{
    vec4 fu_Position;

    vec4 fu_AmbientReflectCoef;
    vec4 fu_DiffuseReflectCoef;
    vec4 fu_SpecularReflectCoef;

    float fu_AttenuationCstTerm;
    float fu_AttenuationLinTerm;
    float fu_AttenuationQuadTerm;

    bool fu_IsBlinn;
} pointLight;

// See C++  class SpotLight
layout (std140) uniform ubo_SpotLight
{
    vec4 fu_Position;
    vec4 fu_Direction;

    vec4 fu_AmbientReflectCoef;
    vec4 fu_DiffuseReflectCoef;
    vec4 fu_SpecularReflectCoef;

    float fu_AttenuationCstTerm;
    float fu_AttenuationLinTerm;
    float fu_AttenuationQuadTerm;

    float fu_Cutoff;
    float fu_OuterCutoff;

    bool fu_IsBlinn;
    bool fu_IsCameraFlashlight;
} spotLight;
uniform bool fu_IsCameraFlashlight;

layout (std140) uniform vec4 ubo_CameraPosition;

vec3 ComputeDirectionalLightPhongIllumination()
{
    // uv-coordinates need to be inversed due to DDS compressing
    vec3 diffuseTex = texture(material.fu_DiffuseTex, vec2(1.0 - vo_TexCoords.x, 1.0 - vo_TexCoords.y)).rgb;

    // Ambient component
    vec3 ambientIntensity = directionalLight.fu_AmbientReflectCoef.xyz * diffuseTex;
        
    // Diffuse component
    vec3 normalDir = normalize(vo_Normal);
    vec3 lightDir = normalize(-directionalLight.fu_Direction.xyz);
    float diffuseImpact = max(0.0, dot(normalDir, lightDir));
    vec3 diffuseIntensity = directionalLight.fu_DiffuseReflectCoef.xyz * diffuseImpact * diffuseTex;
        
    // Specular component
    vec3 viewDir = normalize(ubo_CameraPosition.xyz - vo_Position);
    float specularHighlight = 0.0;
    if(directionalLight.fu_IsBlinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specularHighlight = pow(max(0.0, dot(normalDir, halfwayDir)), material.fu_Shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normalDir);
        specularHighlight = pow(max(0.0, dot(viewDir, reflectDir)), material.fu_Shininess);
    }
    vec3 specularIntensity = directionalLight.fu_SpecularReflectCoef.xyz * specularHighlight * material.fu_SpecularColour;

    return (ambientIntensity + diffuseIntensity + specularIntensity);  
}

vec3 ComputePointLightPhongIllumination()
{
    // uv-coordinates need to be inversed due to DDS compressing
    vec3 diffuseTex = texture(material.fu_DiffuseTex, vec2(1.0 - vo_TexCoords.x, 1.0 - vo_TexCoords.y)).rgb;

    // Ambient component
    vec3 ambientIntensity = pointLight.fu_AmbientReflectCoef.xyz * diffuseTex;
        
    // Diffuse component
    vec3 normalDir = normalize(vo_Normal);
    vec3 lightDir = normalize(pointLight.fu_Position.xyz - vo_Position);
    float diffuseImpact = max(0.0, dot(normalDir, lightDir));
    vec3 diffuseIntensity = pointLight.fu_DiffuseReflectCoef.xyz * diffuseImpact * diffuseTex;
        
    // Specular component
    vec3 viewDir = normalize(ubo_CameraPosition.xyz - vo_Position);
    float specularHighlight = 0.0;
    if(pointLight.fu_IsBlinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specularHighlight = pow(max(0.0, dot(normalDir, halfwayDir)), material.fu_Shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normalDir);
        specularHighlight = pow(max(0.0, dot(viewDir, reflectDir)), material.fu_Shininess);
    }
    vec3 specularIntensity = pointLight.fu_SpecularReflectCoef.xyz * specularHighlight * material.fu_SpecularColour;

    // Attenuation of intensity
    float distFragLight = length(pointLight.fu_Position.xyz - vo_Position);
    float attenuation = 1.0 / (pointLight.fu_AttenuationCstTerm + (pointLight.fu_AttenuationLinTerm * distFragLight) + (pointLight.fu_AttenuationQuadTerm * distFragLight * distFragLight));
    
    return (ambientIntensity + diffuseIntensity + specularIntensity) * attenuation;  
}

vec3 ComputeSpotLightPhongIllumination()
{
    // uv-coordinates need to be inversed due to DDS compressing
    vec3 diffuseTex = texture(material.fu_DiffuseTex, vec2(1.0 - vo_TexCoords.x, 1.0 - vo_TexCoords.y)).rgb;

    // Ambient component
    vec3 ambientIntensity = spotLight.fu_AmbientReflectCoef.xyz * diffuseTex;
        
    // Diffuse component
    vec3 normalDir = normalize(vo_Normal);
    vec3 lightDir = normalize(spotLight.fu_Position.xyz - vo_Position);
    float diffuseImpact = max(0.0, dot(normalDir, lightDir));
    vec3 diffuseIntensity = spotLight.fu_DiffuseReflectCoef.xyz * diffuseImpact * diffuseTex;
        
    // Specular component
    vec3 viewDir = normalize(ubo_CameraPosition.xyz - vo_Position);
    float specularHighlight = 0.0;
    if(spotLight.fu_IsBlinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specularHighlight = pow(max(0.0, dot(normalDir, halfwayDir)), material.fu_Shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normalDir);
        specularHighlight = pow(max(0.0, dot(viewDir, reflectDir)), material.fu_Shininess);
    }
    vec3 specularIntensity = spotLight.fu_SpecularReflectCoef.xyz * specularHighlight * material.fu_SpecularColour;

    // Attenuation of intensity
    float distFragLight = length(spotLight.fu_Position.xyz - vo_Position);
    float attenuation = 1.0 / (spotLight.fu_AttenuationCstTerm + (spotLight.fu_AttenuationLinTerm * distFragLight) + (spotLight.fu_AttenuationQuadTerm * distFragLight * distFragLight));
    
    // SpotLight size/smoothness
    float theta = dot(lightDir, normalize(-spotLight.fu_Direction.xyz)); 
    float epsilon = spotLight.fu_Cutoff - spotLight.fu_OuterCutoff;
    float intensity = clamp((theta - spotLight.fu_OuterCutoff) / epsilon, 0.0, 1.0);

    return (ambientIntensity + diffuseIntensity + specularIntensity) * attenuation * intensity;  
}

void main()
{
    // No directional light contribution for now

    // Point light contribution
    vec3 pointLightPhongIllumination = ComputePointLightPhongIllumination();

    // Spot light contribution
    vec3 spotLightPhongIllumination = vec3(0.0, 0.0, 0.0);
    if (spotLight.fu_IsCameraFlashlight)
    {
        spotLightPhongIllumination = ComputeSpotLightPhongIllumination();
    }

    fo_Colour.xyzw = vec4(pointLightPhongIllumination + spotLightPhongIllumination, material.fu_Transparency);
}