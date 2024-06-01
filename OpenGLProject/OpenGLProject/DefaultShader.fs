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

// See C++ class PointLight
layout (std140) uniform ubo_PointLightParams
{
    vec4 fu_LightPosition;

    vec4 fu_AmbientReflectCoef;
    vec4 fu_DiffuseReflectCoef;
    vec4 fu_SpecularReflectCoef;

    float fu_AttenuationCstTerm;
    float fu_AttenuationLinTerm;
    float fu_AttenuationQuadTerm;

    bool fu_IsBlinn;
};

layout (std140) uniform vec4 ubo_CameraPosition;

void main()
{
    // uv-coordinates need to be inversed due to DDS compressing
    vec3 diffuseTex = texture(material.fu_DiffuseTex, vec2(vo_TexCoords.x, 1.0 - vo_TexCoords.y)).rgb;

    // Ambient component
    vec3 ambientIntensity = fu_AmbientReflectCoef.xyz * diffuseTex;
        
    // Diffuse component
    vec3 normalDir = normalize(vo_Normal);
    vec3 lightDir = normalize(fu_LightPosition.xyz - vo_Position);
    float diffuseImpact = max(0.0, dot(normalDir, lightDir));
    vec3 diffuseIntensity = fu_DiffuseReflectCoef.xyz * diffuseImpact * diffuseTex;
        
    // Specular component
    vec3 viewDir = normalize(ubo_CameraPosition.xyz - vo_Position);
    float specularHighlight = 0.0f;
    if(fu_IsBlinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specularHighlight = pow(max(0.0, dot(normalDir, halfwayDir)), material.fu_Shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normalDir);
        specularHighlight = pow(max(0.0, dot(viewDir, reflectDir)), material.fu_Shininess);
    }
    vec3 specularIntensity = fu_SpecularReflectCoef.xyz * specularHighlight * material.fu_SpecularColour;

    float distFragLight = length(fu_LightPosition.xyz - vo_Position);
    float attenuation = 1.0 / (fu_AttenuationCstTerm + (fu_AttenuationLinTerm * distFragLight) + (fu_AttenuationQuadTerm * distFragLight * distFragLight));
        
    vec3 phongIllumination = (ambientIntensity + diffuseIntensity + specularIntensity) * attenuation;
    fo_Colour.xyzw = vec4(phongIllumination, material.fu_Transparency);
}