#version 330 core

in vec2 texCoord;
out vec4 fColor;

uniform sampler2D texSampler;

void main()
{
    fColor.xyzw = texture(texSampler, vec2(texCoord.x, 1.0 - texCoord.y));
}