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
struct DirectionalLight
{
    vec3 fu_Dir;

    vec3 fu_AmbientReflectCoef;
    vec3 fu_DiffuseReflectCoef;
    vec3 fu_SpecularReflectCoef;

    bool fu_IsBlinn;
};
uniform DirectionalLight light;

layout (std140) uniform vec4 ubo_CameraPosition;

void main()
{
    // uv-coordinates need to be inversed due to DDS compressing
    vec3 diffuseTex = texture(material.fu_DiffuseTex, vec2(vo_TexCoords.x, 1.0 - vo_TexCoords.y)).rgb;

    // Ambient component
    vec3 ambientIntensity = light.fu_AmbientReflectCoef * diffuseTex;
        
    // Diffuse component
    vec3 normalDir = normalize(vo_Normal);
    vec3 lightDir = normalize(-light.fu_Dir.xyz);
    float diffuseImpact = max(0.0, dot(normalDir, lightDir));
    vec3 diffuseIntensity = light.fu_DiffuseReflectCoef * diffuseImpact * diffuseTex;

    // Specular component
    vec3 viewDir = normalize(ubo_CameraPosition.xyz - vo_Position);
    float specularHighlight = 0.0f;
    if(light.fu_IsBlinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        specularHighlight = pow(max(0.0, dot(normalDir, halfwayDir)), material.fu_Shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normalDir);
        specularHighlight = pow(max(0.0, dot(viewDir, reflectDir)), material.fu_Shininess);
    }
    vec3 specularIntensity = light.fu_SpecularReflectCoef * specularHighlight * material.fu_SpecularColour;

    vec3 phongIllumination = ambientIntensity + diffuseIntensity + specularIntensity;
    fo_Colour.xyzw = vec4(phongIllumination, material.fu_Transparency);
}