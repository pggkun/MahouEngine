#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 vtxTexCoord;

uniform mat4 projMtx;


void main()
{
    vec4 pos = aInstanceMatrix * vec4(aPos, 1.0);
    gl_Position = projMtx * pos;
    vtxTexCoord = aTexCoord;
}