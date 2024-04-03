#version 330 core

layout (location = 0) in vec3 vPos;

out vec3 texDir;

layout (std140) uniform matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
	texDir.xyz = vPos.xyz;

	vec4 pos = projection * view * vec4(vPos.xyz, 1.0f);
    gl_Position.xyzw = pos.xyww;
}