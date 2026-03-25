#version 460 core

struct VertexPosition {
    vec3 position;
    uint packedIndex;
};

layout(std430, binding = 0) readonly buffer PosBuffer {
    VertexPosition positions[];
};

struct VertexAttribute {
    vec3 normal;
    float uv_x;
    vec3 tangent;
    float uv_y;
};

layout(std430, binding = 1) readonly buffer AttrBuffer {
    VertexAttribute attributes[];
};

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;
uniform mat4 modelIT;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUv;

void main() 
{
    VertexPosition vertPos = positions[gl_VertexID];
    VertexAttribute vertAttr = attributes[gl_VertexID];

    vec4 worldPos = model * vec4(vertPos.position, 1.0); 
    
    outPos = worldPos.xyz;
    outNormal = normalize((modelIT * vec4(vertAttr.normal, 0)).xyz); 
    outUv = vec2(vertAttr.uv_x, vertAttr.uv_y);

    gl_Position = proj * view * worldPos;
}