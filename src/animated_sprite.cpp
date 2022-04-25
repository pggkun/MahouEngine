#include "animated_sprite.h"
#include <stdio.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

GLuint AnimatedSprite::CreateAndCompileShader(GLenum type, const char *source)
{
    GLint success;
    GLchar msg[512];

    GLuint handle = glCreateShader(type);
    if (!handle)
    {
        //TRACE("%u: cannot create shader", type);
        return 0;
    }
    glShaderSource(handle, 1, &source, nullptr);
    glCompileShader(handle);
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(handle, sizeof(msg), nullptr, msg);
        //TRACE("%u: %s\n", type, msg);
        glDeleteShader(handle);
        return 0;
    }

    return handle;
}

AnimatedSprite::~AnimatedSprite()
{
    glDeleteBuffers(1, &this->s_vbo);
    glDeleteVertexArrays(1, &this->s_vao);
    glDeleteProgram(this->s_program);
    glDeleteTextures(1, &this->s_tex);
}

void AnimatedSprite::ApplyTexture(const void *file, int len)
{
    int width, height, nchan;
    // stbi_set_flip_vertically_on_load(true);
    stbi_uc *img = stbi_load_from_memory((stbi_uc *)file, len, &width, &height, &nchan, 4);
    assert(img);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    stbi_image_free(img);
}

AnimatedSprite::AnimatedSprite(int frmamt_x, int frmamt_y, float fldur, const void *file, int len)
{
    this->animations.push_back(std::vector<int>{0,0});

    this->frame_amount_x = frmamt_x;
    this->frame_amount_y = frmamt_y;
    this->frame_duration = fldur;
    GLint vsh = CreateAndCompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLint fsh = CreateAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    this->s_program = glCreateProgram();
    glAttachShader(this->s_program, vsh);
    glAttachShader(this->s_program, fsh);
    glLinkProgram(this->s_program);

    GLint success;
    glGetProgramiv(this->s_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char buf[512];
        glGetProgramInfoLog(this->s_program, sizeof(buf), nullptr, buf);
        //TRACE("Link error: %s", buf);
    }
    glDeleteShader(vsh);
    glDeleteShader(fsh);

    this->loc_mdlvMtx = glGetUniformLocation(this->s_program, "mdlvMtx");
    this->loc_projMtx = glGetUniformLocation(this->s_program, "projMtx");
    this->loc_tex_diffuse = glGetUniformLocation(this->s_program, "tex_diffuse");
    this->loc_color_diffuse = glGetUniformLocation(this->s_program, "color");

    glGenVertexArrays(1, &this->s_vao);
    glGenBuffers(1, &this->s_vbo);
    glBindVertexArray(this->s_vao);

    this->current_animation_index = 0;
    float vtx = StartX(this->animations[current_animation_index][0] % (animations[current_animation_index].size()));
    float vtxE = EndX(this->animations[current_animation_index][0] % (animations[current_animation_index].size()));
    float vty = StartY((int)(this->animations[current_animation_index][0] / (animations[current_animation_index].size())));
    float vtyE = EndY((int)(this->animations[current_animation_index][0] / (animations[current_animation_index].size())));
    UpdateFrameData(vtx, vtxE, vty, vtyE);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->s_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->data), this->data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);

    glBindVertexArray(0);

    glGenTextures(1, &this->s_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->s_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    ApplyTexture((stbi_uc *)file, len);

    glUseProgram(this->s_program);
    this->sprite_color = glm::vec4{1,1,1,1};
    glUniform4f(this->loc_color_diffuse, sprite_color.r, sprite_color.g, sprite_color.b, sprite_color.a);
    glUniform1i(this->loc_tex_diffuse, 0);
}

float AnimatedSprite::StartX(int sheet_position)
{
    return (1.0 / this->frame_amount_x) * sheet_position;
}

float AnimatedSprite::EndX(int sheet_position)
{
    return (1.0 / this->frame_amount_x) * (sheet_position + 1);
}

float AnimatedSprite::StartY(int sheet_position)
{
    return (1.0 / this->frame_amount_y) * sheet_position;
}

float AnimatedSprite::EndY(int sheet_position)
{
    return (1.0 / this->frame_amount_y) * (sheet_position + 1);
}

void AnimatedSprite::UpdateFrameData(float x_start, float x_end, float y_start, float y_end)
{
    Vertex vertices[6] =
        {
            {{-0.5f, -0.5f, 0.0f}, {x_start, y_end}},
            {{0.5f, -0.5f, 0.0f}, {x_end, y_end}},
            {{-0.5f, 0.5f, 0.0f}, {x_start, y_start}},

            {{0.5f, -0.5f, 0.0f}, {x_end, y_end}},
            {{0.5f, 0.5f, 0.0f}, {x_end, y_start}},
            {{-0.5f, 0.5f, 0.0f}, {x_start, y_start}},
        };
    memcpy(this->data, vertices, sizeof(vertices));
}

void AnimatedSprite::UpdateAnimation()
{
    if ((animation_time) > (this->frame_duration * animations[current_animation_index].size()))
    {
        this->current_game_frame = 0;
        animation_time = 0;
    }

    int my_frame = (int)floor((animation_time) / this->frame_duration);
    float vtx = StartX(this->animations[current_animation_index][my_frame] % (animations[current_animation_index].size()));
    float vtxE = EndX(this->animations[current_animation_index][my_frame] % (animations[current_animation_index].size()));
    float vty = StartY((int)(this->animations[current_animation_index][my_frame] / (animations[current_animation_index].size())));
    float vtyE = EndY((int)(this->animations[current_animation_index][my_frame] / (animations[current_animation_index].size())));
    UpdateFrameData(vtx, vtxE, vty, vtyE);
    glBindBuffer(GL_ARRAY_BUFFER, this->s_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->data), this->data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->current_game_frame++;
    animation_time += GameTime::delta_time;
}

void AnimatedSprite::PushAnimation(std::vector<int> frames)
{
    this->animations.push_back(frames);
}

void AnimatedSprite::SwitchAnimation(int animation_index, float duration)
{
    this->current_animation_index = animation_index;
    this->frame_duration = duration;
}

void AnimatedSprite::Draw()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->s_tex);
    glUniform4f(this->loc_color_diffuse, this->sprite_color.r, this->sprite_color.g, this->sprite_color.b, this->sprite_color.a);
    glUniform1i(this->loc_tex_diffuse, 0);
    glBindVertexArray(this->s_vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->s_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->data), this->data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //printf("anbtes: %d\n", GameTime::draw_calls);
    GameTime::draw_calls += 1;
    //printf("depois: %d\n", GameTime::draw_calls);
    glBindVertexArray(0);
} 