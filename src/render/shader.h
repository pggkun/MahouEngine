#ifndef SHADER_H
#define SHADER_H

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include "../GLEW/glew.h"
#endif

#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public:
        unsigned int ID;

        Shader(const char *vertexCode, const char *fragmentCode, const char *geometryCode);

        void use();
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec2(const std::string &name, float x, float y) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3(const std::string &name, float x, float y, float z) const;
        void setVec4(const std::string &name, const glm::vec4 &value) const;
        void setVec4(const std::string &name, float x, float y, float z, float w);
        void setMat2(const std::string &name, const glm::mat2 &mat) const;
        void setMat3(const std::string &name, const glm::mat3 &mat) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
    private:
        void checkCompileErrors(GLuint shader, std::string type);
};
#endif