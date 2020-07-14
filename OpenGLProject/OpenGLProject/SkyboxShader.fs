#version 330 core

in vec3 texDir;   
out vec4 fColor;             

uniform samplerCube texSampler;   

void main()
{
    fColor.xyzw = texture(texSampler, texDir.xyz);
}