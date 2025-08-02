#version 450

layout(location = 0) in uint inVertex;
layout(location = 1) in vec3 inPosition;
layout(location = 2) in float inRotation;
layout(location = 0) out flat vec3 outNormal;
layout(location = 1) out vec2 outTexcoord;
layout(set = 1, binding = 0) uniform uniformViewProjMatrix
{
    mat4 viewProjMatrix;
};

vec3 GetPosition(uint vertex)
{
    vec3 magnitude;
    vec3 direction;
    magnitude.x = float((vertex >> 0) & 0x3Fu);
    direction.x = float((vertex >> 6) & 0x1u);
    magnitude.y = float((vertex >> 7) & 0x3Fu);
    direction.y = float((vertex >> 13) & 0x1u);
    magnitude.z = float((vertex >> 14) & 0x3Fu);
    direction.z = float((vertex >> 20) & 0x1u);
    return (1.0f - 2.0f * direction) * magnitude;
}

const vec3 kNormals[6] = vec3[6]
(
    vec3(-1.0f, 0.0f, 0.0f ),
    vec3( 1.0f, 0.0f, 0.0f ),
    vec3( 0.0f,-1.0f, 0.0f ),
    vec3( 0.0f, 1.0f, 0.0f ),
    vec3( 0.0f, 0.0f,-1.0f ),
    vec3( 0.0f, 0.0f, 1.0f )
);

vec3 GetNormal(uint vertex)
{
    return kNormals[(vertex >> 21) & 0x7u];
}

vec2 GetTexcoord(uint vertex)
{
    return vec2(float((vertex >> 24) & 0xFFu) / 255.0f, 0.5f);
}

mat3 GetRotationMatrix(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return mat3(c, 0.0f, s, 0.0f, 1.0f, 0.0f, -s, 0.0f, c);
}

void main()
{
    mat3 rotationMatrix = GetRotationMatrix(inRotation);
    vec3 position = inPosition + rotationMatrix * GetPosition(inVertex);
    outNormal = rotationMatrix * GetNormal(inVertex);
    outTexcoord = GetTexcoord(inVertex);
    gl_Position = viewProjMatrix * vec4(position, 1.0f);
}