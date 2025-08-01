#version 450

layout(location = 0) out vec4 outColor;
layout(location = 0) in flat vec3 inNormal;
layout(location = 1) in vec2 inTexcoord;
layout(set = 2, binding = 0) uniform sampler2D paletteTexture;

void main()
{
    outColor = texture(paletteTexture, inTexcoord);
}