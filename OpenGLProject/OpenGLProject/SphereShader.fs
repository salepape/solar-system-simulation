#version 330 core

in vec2 tex;   
out vec4 fragColor;             // Fragment Color

uniform sampler2D texSphere;   

void main()
{
    fragColor = texture(texSphere, tex);
}