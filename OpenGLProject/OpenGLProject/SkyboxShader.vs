#version 330 core

layout (location = 0) in vec3 va_Position;

out vec3 vo_TexDir;

layout (std140) uniform matrices
{
    mat4 vu_Projection;
    mat4 vu_View;
};

void main()
{
	vo_TexDir.xyz = va_Position.xyz;

	vec4 position = vu_Projection * vu_View * vec4(va_Position.xyz, 1.0f);
    gl_Position.xyzw = position.xyww;
}