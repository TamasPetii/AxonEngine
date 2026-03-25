#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUv;

layout(location = 0) out vec4 FragColor;

struct Material 
{
    vec4 baseColor;
    uint64_t albedoHandle;
    uint64_t normalHandle;
};

layout(std430, binding = 2) readonly buffer MaterialBuffer 
{
    Material materials[];
};

uniform uint u_MaterialIndex;

void main() 
{
    Material mat = materials[u_MaterialIndex];  
    vec4 finalColor = mat.baseColor;

    if (mat.albedoHandle != 0) 
    {
        sampler2D albedoMap = sampler2D(mat.albedoHandle);
        finalColor *= texture(albedoMap, inUv); 
    }
    
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    float diff = max(dot(normalize(inNormal), lightDir), 0.2); 
    
    FragColor = vec4(finalColor.rgb * diff, finalColor.a);
}