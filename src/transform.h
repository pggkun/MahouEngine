#ifndef _TANSFORM_H_
#define _TANSFORM_H_

#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <gtc/type_ptr.hpp>

class Transform
{
    public:
        Transform();
        ~Transform();
        glm::vec3 position;
        float angle;
        glm::vec3 rotationAxis;
        glm::vec3 scaleAmount;

        glm::mat4 translate();
        glm::mat4 rotate();
        glm::mat4 scale();
        glm::mat4 modelMatrix();
};

#endif
