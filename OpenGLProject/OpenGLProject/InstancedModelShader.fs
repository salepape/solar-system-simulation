#version 330 core

in vec3 fPos;
in vec3 normalCoord;
in vec2 texCoord;

out vec4 fColor;

layout (std140) uniform materialParameters
{
    vec4 materialSpecular;
    float materialShininess;
};
uniform sampler2D materialDiffuse;

layout (std140) uniform lightParameters
{
    vec4 lightPosition;

    // Terms of Phong shading formula
    vec4 lightAmbient;
    vec4 lightDiffuse;
    vec4 lightSpecular;

    // Terms of attenuation spotlight formula
    float lightConstant;
    float lightLinear;
    float lightQuadratic;

    bool isBlinn;
};

uniform vec3 viewPos;

void main()
{
    // uv coordinates need to be inversed due to DDS textures use
    vec3 texelSampling = texture(materialDiffuse, vec2(texCoord.x, 1.0 - texCoord.y)).rgb;

    // Ambient lighting
    vec3 ambient = lightAmbient.xyz * texelSampling;
    
    // Diffuse lighting
    vec3 normalDir = normalize(normalCoord);
    vec3 lightDir = normalize(lightPosition.xyz - fPos);
    float diff = max(dot(normalDir, lightDir), 0.0);
    vec3 diffuse = lightDiffuse.xyz * diff * texelSampling;
    
    // Specular lighting
    vec3 viewDir = normalize(viewPos - fPos);
    float spec = 0.0f;
    if(isBlinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normalDir, halfwayDir), 0.0), materialShininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normalDir);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    }
    vec3 specular = lightSpecular.xyz * (spec * materialSpecular.xyz);

    float distance = length(lightPosition.xyz - fPos);
    float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));
    
    vec3 result = (ambient + diffuse + specular) * attenuation;
    fColor.xyzw = vec4(result, 1.0);
}