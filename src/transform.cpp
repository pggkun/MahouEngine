#include "transform.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

glm::mat4 Transform::translate()
{
    return glm::translate(glm::mat4(1.0), this->position);
}

glm::mat4 Transform::rotate()
{
    return glm::rotate(this->translate(), this->angle, this->rotationAxis);
}

glm::mat4 Transform::scale()
{
    return glm::scale(this->rotate(), this->scaleAmount);
}

glm::mat4 Transform::modelMatrix()
{
    return this->scale();
}