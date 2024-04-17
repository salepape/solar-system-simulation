#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 2) in vec2 vTexCoord;

out vec3 fPos;
out vec2 texCoord;

layout (std140) uniform matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

void main()
{
    texCoord.xy = vTexCoord.xy;

	vec3 fModelPos = vec3(model * vec4(vPos.xyz, 1.0));
	gl_Position.xyzw = projection * view * vec4(fModelPos.xyz, 1.0f);
}