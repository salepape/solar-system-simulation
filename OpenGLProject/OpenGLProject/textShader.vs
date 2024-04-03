#version 330 core

layout(location = 0) in vec4 vertex;		// <vec2 vPos, vec2 vTexCoord>

out vec2 texCoord;

layout (std140) uniform matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

void main()
{
	texCoord = vertex.zw;
	
	gl_Position = projection * view * model * vec4(vertex.xy, 0.0f, 1.0f);
}