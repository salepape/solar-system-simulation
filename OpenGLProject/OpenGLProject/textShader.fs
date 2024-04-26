#version 330 core

in vec2 vo_TexCoords;

out vec4 fo_Colour;

uniform sampler2D fu_TexSampler;
uniform vec3 fu_TexColour;

void main()
{
	// Texture's data is stored in just its red component, we sample the r component of the texture as the sampled alpha value 
	// (resulting pixel will be transparent for all the glyph's background colours and non-transparent for the actual character pixels)
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(fu_TexSampler, vo_TexCoords).r);
	fo_Colour = vec4(fu_TexColour, 1.0f) * sampled;
}