#version 330 core

in vec3 vo_Position;
in vec3 vo_Normal;
in vec2 vo_TexCoords;

out vec4 fo_Colour;

// See C++ class Material
uniform sampler2D fu_DiffuseMat;
layout (std140) uniform specularMatParams
{
    vec4 fu_SpecularColour;

    float fu_Shininess;
};

// See C++ class DirectionalLight
layout (std140) uniform directionalLightParams
{
    vec4 fu_LightDir;

    vec4 fu_AmbientReflectCoef;
    vec4 fu_DiffuseReflectCoef;
    vec4 fu_SpecularReflectCoef;

    bool fu_IsBlinn;
};

uniform vec3 fu_viewPosition;

void main()
{
    // uv-coordinates need to be inversed due to DDS compressing
    vec3 diffuseTex = texture(fu_DiffuseMat, vec2(vo_TexCoords.x, 1.0 - vo_TexCoords.y)).rgb;

    // Ambient component
    vec3 ambientIntensity = fu_AmbientReflectCoef.xyz * diffuseTex;
        
    // Diffuse component
    vec3 normalDir = normalize(vo_Normal);
    vec3 lightDir = normalize(-fu_LightDir.xyz);
    float diffuseImpact = max(0.0, dot(normalDir, lightDir));
    vec3 diffuseIntensity = fu_DiffuseReflectCoef.xyz * diffuseImpact * diffuseTex;

    // Specular component
    vec3 viewDir = normalize(fu_viewPosition - vo_Position);
    float specularHighlight = 0.0f;
    if(fu_IsBlinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specularHighlight = pow(max(0.0, dot(normalDir, halfwayDir)), fu_Shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normalDir);
        specularHighlight = pow(max(0.0, dot(viewDir, reflectDir)), fu_Shininess);
    }
    vec3 specularIntensity = fu_SpecularReflectCoef.xyz * specularHighlight * fu_SpecularColour.xyz;

    vec3 phongIllumination = ambientIntensity + diffuseIntensity + specularIntensity;
    fo_Colour.xyzw = vec4(phongIllumination, 1.0);
}