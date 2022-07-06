#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include "shader.h"

class Texture
{
    public:
        Texture(const void *file, int len);
        Texture(char const * file);
        ~Texture();
        unsigned char *image;
        int width, height, nchan;
        GLuint textureID;
        void GenerateTexture(GLint filter);
};

#endif