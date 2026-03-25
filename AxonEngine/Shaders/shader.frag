#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec4 outColor;

uniform vec3 u_Color;


void main() 
{
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 ambient = 0.2 * lightColor;

    vec3 norm = normalize(inNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * u_Color;
    outColor = vec4(result, 1.0);
}