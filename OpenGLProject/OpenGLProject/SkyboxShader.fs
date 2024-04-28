#version 330 core

in vec3 vo_TexDir;
out vec4 fo_Colour;

uniform samplerCube fu_DiffuseMat;

void main()
{
    fo_Colour.xyzw = texture(fu_DiffuseMat, vo_TexDir.xyz);
}