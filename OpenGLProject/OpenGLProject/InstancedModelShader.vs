#version 330 core

layout (location = 0) in vec3 va_Position;
layout (location = 1) in vec3 va_Normal;
layout (location = 2) in vec2 va_TexCoords;
layout (location = 5) in mat4 va_InstanceMatrix;		// locations 5, 6, 7 and 8 reserved for each column of the matrix

out vec3 vo_Position;
out vec3 vo_Normal;
out vec2 vo_TexCoords;

layout (std140) uniform ubo_ProjectionView
{
    mat4 vu_ProjectionView;
};

void main()
{
	vo_Position.xyz = vec3(va_InstanceMatrix * vec4(va_Position.xyz, 1.0));
	vo_Normal.xyz = mat3(transpose(inverse(va_InstanceMatrix))) * va_Normal.xyz;
    vo_TexCoords.xy = va_TexCoords.xy;

	gl_Position.xyzw = vu_ProjectionView * va_InstanceMatrix * vec4(va_Position.xyz, 1.0f);
}