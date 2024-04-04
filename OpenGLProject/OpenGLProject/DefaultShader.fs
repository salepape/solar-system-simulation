#version 330 core

in vec3 fPos;
in vec3 normalCoord;
in vec2 texCoord;

out vec4 fColor;

layout (std140) uniform materialParameters
{
    vec3 materialSpecular;
    float materialShininess;
};
uniform sampler2D materialDiffuse;

struct Light
{
    vec3 position;

    // Terms of Phong shading formula
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool isBlinn;

    // Terms of attenuation spotlight formula
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;
uniform Light light;
uniform bool isSun;

void main()
{
    if(!isSun)
    {
        // uv coordinates need to be inversed due to DDS textures use
        vec3 texelSampling = texture(materialDiffuse, vec2(texCoord.x, 1.0 - texCoord.y)).rgb;

        // Ambient lighting
        vec3 ambient = light.ambient * texelSampling;
        
        // Diffuse lighting
        vec3 normalDir = normalize(normalCoord);
        vec3 lightDir = normalize(light.position - fPos);
        float diff = max(dot(normalDir, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texelSampling;
        
        // Specular lighting
        vec3 viewDir = normalize(viewPos - fPos);
        float spec = 0.0f;
        if(light.isBlinn)
        {
            vec3 halfwayDir = normalize(lightDir + viewDir);
            spec = pow(max(dot(normalDir, halfwayDir), 0.0), materialShininess);
        }
        else
        {
            vec3 reflectDir = reflect(-lightDir, normalDir);
            spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
        }
        vec3 specular = light.specular * (spec * materialSpecular);

        float distance = length(light.position - fPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        vec3 result = (ambient + diffuse + specular) * attenuation;
        fColor.xyzw = vec4(result, 1.0);
    }
    else
    {
        fColor.xyzw = texture(materialDiffuse, vec2(texCoord.x, 1.0 - texCoord.y));
    }
}