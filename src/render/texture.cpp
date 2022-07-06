#include "texture.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stb_image.h>

const char *ReadFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content.c_str();
}

void Texture::GenerateTexture(GLint filter)
{
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

Texture::Texture(const void *file, int len)
{
    image = stbi_load_from_memory((stbi_uc *)file, len, &width, &height, &nchan, 4);
}

Texture::Texture(char const *file)
{
    const char *file_content = ReadFile(file);
    image = stbi_load(file_content, &width, &height, &nchan, 3);
}

Texture::~Texture()
{
    stbi_image_free(image);
}