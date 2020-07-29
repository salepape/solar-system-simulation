#version 330 core

in vec2 texCoord;
out vec4 fColor;

uniform sampler2D texSampler;
uniform vec3 textColor;

void main()
{
	// Texture's data is stored in just its red component, we sample the r component of the texture as the sampled alpha value 
	// (resulting pixel will be transparent for all the glyph's background colors and non-transparent for the actual character pixels)
	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(texSampler, texCoord).r);
	fColor = vec4(textColor, 1.0f) * sampled;
}