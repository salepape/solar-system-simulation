#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in mat4 vInstanceMatrix;

out vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texCoord.xy = vTexCoord.xy;
    gl_Position.xyzw = projection * view * vInstanceMatrix * vec4(vPos.xyz, 1.0f); 
}