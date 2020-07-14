#version 330 core

in vec2 texCoord;   
out vec4 fColor;             

uniform sampler2D texSampler;  
//uniform sampler2D texAlphaSampler;

void main()
{
    // uv coordinates need to be inversed due to DDS textures use
    vec2 inverseCoord = vec2(texCoord.x, 1.0 - texCoord.y);
    fColor.xyzw = texture(texSampler, inverseCoord);
    //fColor.xyzw = mix(texture(texSampler, inverseCoord), texture(texAlphaSampler, inverseCoord), 0.5);
}