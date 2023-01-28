#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 vtxTexCoord;

uniform mat4 mdlvMtx;
uniform mat4 projMtx;


void main()
{
    vec4 pos = mdlvMtx * vec4(aPos, 1.0);
    gl_Position = projMtx * pos;
    vtxTexCoord = aTexCoord;
}