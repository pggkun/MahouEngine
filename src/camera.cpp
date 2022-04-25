#include "camera.h"

Camera::Camera()
{    
}

Camera::~Camera()
{
}

glm::mat4 Camera::viewMatrix()
{
    return glm::lookAt(this->eye, this->center, this->up);
}

glm::mat4 Camera::projMatrix()
{
    return glm::perspective(this->aperture, this->ratio, this->cnear, this->cfar);
}