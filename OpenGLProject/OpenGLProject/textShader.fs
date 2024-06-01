#version 330 core

in vec2 vo_TexCoords;

out vec4 fo_Colour;

struct Material
{ 
    sampler2D fu_DiffuseTex;
    vec3 fu_DiffuseColour;
};
uniform Material material;

void main()
{
	// FreeType glyph bitmap colours have been stored in their red component (just black/white) 
    float diffuseTexColour = texture(material.fu_DiffuseTex, vo_TexCoords).r;

    // We want each pixel to be transparent for glyph background colours and visible for character colour, 
    // so we sample the colour above as the alpha value
	fo_Colour = vec4(material.fu_DiffuseColour, 1.0f) * vec4(1.0f, 1.0f, 1.0f, diffuseTexColour);
}