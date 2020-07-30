#version 330 core

in vec3 fPos;
in vec3 normalCoord;
in vec2 texCoord;   

out vec4 fColor;             

struct Material 
{
    sampler2D diffuse;      // equivalent to classical code line : uniform sampler2D texSampler;   
    vec3 specular;    
    float shininess;
}; 

struct Light 
{
    vec3 position;

    // Terms of Phong shading formula
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Terms of atenuation spotlight formula
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool isSun;

void main()
{
    if(!isSun)
    {
        // uv coordinates need to be inversed due to DDS textures use
        vec3 texelSampling = texture(material.diffuse, vec2(texCoord.x, 1.0 - texCoord.y)).rgb;

        // Ambient lighting
        vec3 ambient = light.ambient * texelSampling;
    
        // Diffuse lighting
        vec3 norm = normalize(normalCoord);
        vec3 lightDir = normalize(light.position - fPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texelSampling;
    
        // Specular lighting
        vec3 viewDir = normalize(viewPos - fPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * material.specular);  

        float distance = length(light.position - fPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
        
        vec3 result = (ambient + diffuse + specular) * attenuation;
        fColor.xyzw = vec4(result, 1.0);
    }
    else
        fColor.xyzw = texture(material.diffuse, vec2(texCoord.x, 1.0 - texCoord.y));
}


