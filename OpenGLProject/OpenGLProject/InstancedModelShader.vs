#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormalCoord;
layout (location = 2) in vec2 vTexCoord;
layout (location = 5) in mat4 vInstanceMatrix;	// locations 5, 6, 7 and 8 reserved for each column of the matrix

out vec3 fPos;
out vec3 normalCoord;
out vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	fPos.xyz = vec3(vInstanceMatrix * vec4(vPos.xyz, 1.0));
	normalCoord.xyz = mat3(transpose(inverse(vInstanceMatrix))) * vNormalCoord.xyz;
    texCoord.xy = vTexCoord.xy;

	gl_Position.xyzw = projection * view * vInstanceMatrix * vec4(vPos.xyz, 1.0f);
}