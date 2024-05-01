#version 330 core

layout (location = 0) in vec3 va_Position;

out vec3 vo_TexDir;

layout (std140) uniform ubo_ProjectionView
{
    mat4 vu_ProjectionView;
};

void main()
{
	vo_TexDir.xyz = va_Position.xyz;

	vec4 position = vu_ProjectionView * vec4(va_Position.xyz, 1.0f);
    gl_Position.xyzw = position.xyww;
}