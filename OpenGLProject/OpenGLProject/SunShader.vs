#version 330 core

layout (location = 0) in vec3 va_Position;
layout (location = 1) in vec3 va_Normal;
layout (location = 2) in vec2 va_TexCoords;

out vec3 vo_Position;
out vec3 vo_Normal;
out vec2 vo_TexCoords;

layout (std140) uniform matrices
{
    mat4 vu_Projection;
    mat4 vu_View;
};
uniform mat4 vu_Model;

void main()
{
	vo_Position.xyz = vec3(vu_Model * vec4(va_Position.xyz, 1.0));
	vo_Normal.xyz = mat3(transpose(inverse(vu_Model))) * va_Normal.xyz;
    vo_TexCoords.xy = va_TexCoords.xy;

	gl_Position.xyzw = vu_Projection * vu_View * vec4(vo_Position.xyz, 1.0f);
}