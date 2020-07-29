#version 330 core

layout (location = 0) in vec3 vPos;				// -> POSITION 0 FOR VERT COOR
layout (location = 1) in vec3 vNormalCoord;		// -> POSITION 1 FOR NORMAL COOR
layout (location = 2) in vec2 vTexCoord;		// -> POSITION 2 FOR TEXT COOR

out vec3 fPos;
out vec3 normalCoord;
out vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	fPos.xyz = vec3(model * vec4(vPos.xyz, 1.0));
	normalCoord.xyz = mat3(transpose(inverse(model))) * vNormalCoord.xyz;
    texCoord.xy = vTexCoord.xy;

	gl_Position.xyzw = projection * view * vec4(fPos.xyz, 1.0f);
}