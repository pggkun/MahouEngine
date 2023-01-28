#version 330 core

in vec2 vtxTexCoord;

out vec4 fragColor;
uniform sampler2D tex_diffuse;
uniform vec4 color;

void main()
{
    vec4 texDiffuseColor = texture(tex_diffuse, vtxTexCoord);

    fragColor = texDiffuseColor * color;
}