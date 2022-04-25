#pragma once
#ifndef _TEXT_RENDER_H_
#define _TEXT_RENDER_H_

#include <gl/glcorearb.h>
#include <gl/glext.h>
#include <string>
#include <map>
#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <gtc/type_ptr.hpp>
#include "render/shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Offset to advance to next glyph
};

const char *const textVertexShaderSource = R"text(
    #version 330 core
    layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
    out vec2 TexCoords;

    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
    }
)text";

const char *const textFragmentShaderSource = R"text(
    #version 330 core
    in vec2 TexCoords;
    out vec4 color;

    uniform sampler2D text;
    uniform vec3 textColor;

    void main()
    {    
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        color = vec4(textColor, 1.0) * sampled;
    }
)text";

class TextRenderer
{
    private:
        FT_Library ft;
        FT_Face face;
    public:
        TextRenderer(char *font_name, int font_size);
        ~TextRenderer();

        char *text;
        unsigned int VAO, VBO;
        std::map<char, Character> Characters;
        void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
};

#endif