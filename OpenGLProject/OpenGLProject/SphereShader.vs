#version 330 core
layout (location = 0) in vec3 vPos;				// -> POSITION 0 FOR VERT COOR
layout (location = 1) in vec2 vTexCoord;		// -> POSITION 1 FOR TEXT COOR

out vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(vPos, 1.0f);
    texCoord = vTexCoord;
}