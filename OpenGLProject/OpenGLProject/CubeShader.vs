#version 330 core
layout (location = 0) in vec3 vPos;				

out vec3 texDir;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	texDir = vPos;
	vec4 pos = projection * view * vec4(vPos, 1.0f);
    gl_Position = pos.xyww;
}