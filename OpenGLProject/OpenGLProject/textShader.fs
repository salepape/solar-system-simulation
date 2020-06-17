#version 330 core

in vec2 texCoord;
out vec4 fColor;

uniform sampler2D texSampler;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(texSampler, texCoord).r);
    fColor = vec4(textColor, 1.0f) * sampled;
}