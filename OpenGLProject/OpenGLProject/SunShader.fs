#version 330 core

in vec2 vo_TexCoords;

out vec4 fo_Colour;

// See C++ class Material
struct Material
{
    sampler2D fu_DiffuseTex;
    vec3 fu_DiffuseColour;
};
uniform Material material;

void main()
{
    // uv-coordinates need to be inversed due to DDS compressing
    vec4 diffuseTex = texture(material.fu_DiffuseTex, vec2(1.0 - vo_TexCoords.x, 1.0 - vo_TexCoords.y));

    fo_Colour.xyzw = vec4(material.fu_DiffuseColour, 1.0) * diffuseTex;
}