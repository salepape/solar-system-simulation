#version 330 core

in vec3 vo_TexDir;
out vec4 fo_Colour;

struct Material
{
    samplerCube fu_DiffuseTex_0;
};
uniform Material material;

void main()
{
    fo_Colour.xyzw = texture(material.fu_DiffuseTex_0, vo_TexDir.xyz);
}