#version 330 core

in vec2 texCoord;
out vec4 fColor;

uniform sampler2D texSampler;

void main()
{
    fColor.xyzw = texture(texSampler, texCoord.xy);
}