#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm.hpp>
#include <ext/matrix_clip_space.hpp>
#include <gtc/type_ptr.hpp>

class Camera
{
    public:
        Camera();
        ~Camera();
        glm::vec3 eye;
        glm::vec3 center;
        glm::vec3 up;
        float aperture;
        float ratio;
        float cnear;
        float cfar;

        glm::mat4 viewMatrix();
        glm::mat4 projMatrix();        
};

#endif