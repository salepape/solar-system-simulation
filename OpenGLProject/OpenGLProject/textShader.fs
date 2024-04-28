#version 330 core

in vec2 vo_TexCoords;

out vec4 fo_Colour;

uniform sampler2D fu_DiffuseMat;
uniform vec3 fu_DiffuseColour;

void main()
{
	// FreeType glyph bitmap has only been stored in its red component, so we sample it as the alpha value 
	// (resulting pixel will be transparent for glyph background colours and visible for character colour)
    float diffuseTexColour = texture(fu_DiffuseMat, vo_TexCoords).r;
	fo_Colour = vec4(fu_DiffuseColour, 1.0f) * vec4(1.0f, 1.0f, 1.0f, diffuseTexColour);
}