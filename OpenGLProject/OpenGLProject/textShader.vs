#version 330 core

layout(location = 0) in vec4 va_Quad;		// <vec2 va_Position, vec2 va_TexCoords>

out vec2 vo_TexCoords;

layout (std140) uniform ubo_ProjectionView
{
    mat4 vu_ProjectionView;
};
uniform mat4 vu_Model;

void main()
{
	vo_TexCoords = va_Quad.zw;
	
	gl_Position = vu_ProjectionView * vu_Model * vec4(va_Quad.xy, 0.0, 1.0);
}