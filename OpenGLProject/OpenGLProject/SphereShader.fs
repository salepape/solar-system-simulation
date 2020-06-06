#version 330 core

in vec2 texCoord;   
out vec4 fColor;             // Fragment Color

uniform sampler2D texSampler;   

void main()
{
    fColor = texture(texSampler, texCoord);
}