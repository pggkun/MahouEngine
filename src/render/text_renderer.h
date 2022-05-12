#pragma once
#ifndef _TEXT_RENDER_H_
#define _TEXT_RENDER_H_

#include <string>
#include <map>
#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"
#include <ft2build.h>

#include <text_vert_shader.h>
#include <text_frag_shader.h>

#include FT_FREETYPE_H

struct Character
{
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

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