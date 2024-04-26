#version 330 core

layout(location = 0) in vec4 va_Quad;		// <vec2 va_Position, vec2 va_TexCoords>

out vec2 vo_TexCoords;

layout (std140) uniform matrices
{
    mat4 vu_Projection;
    mat4 vu_View;
};
uniform mat4 vu_Model;

void main()
{
	vo_TexCoords = va_Quad.zw;
	
	gl_Position = vu_Projection * vu_View * vu_Model * vec4(va_Quad.xy, 0.0f, 1.0f);
}