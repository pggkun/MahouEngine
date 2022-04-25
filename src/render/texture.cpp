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