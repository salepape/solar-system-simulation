#version 330 core

in vec3 vo_TexDir;
out vec4 fo_Colour;

uniform samplerCube fu_TexSampler;

void main()
{
    fo_Colour.xyzw = texture(fu_TexSampler, vo_TexDir.xyz);
}