#include "game_object.h"

GameObject::GameObject()
{
    transform = new Transform();
    transform->position = glm::vec3{0, 0, 0};
    transform->rotationAxis = glm::vec3{0, 0, -1};
    transform->angle = 0;
    transform->scaleAmount = glm::vec3{1, 1, 1};
}

GameObject::~GameObject()
{
    delete transform;
    delete mesh;
    delete sprite;
}

void GameObject::SetupPlane(float x, float y, float w, float h)
{
    mesh = new Mesh();
    mesh->SetupPlane(0,1,0,1,x,y,w,h);
    mesh->Initialize();
}

void GameObject::MoveTo(float x, float y)
{
    transform->position = glm::vec3{x, y, transform->position.z};
}

void GameObject::MoveTo(float x, float y, float z)
{
    transform->position = glm::vec3{x, y, z};
}

void GameObject::AssignMaterial(Material *m)
{
    material = m;
    m->objectsToDraw.push_back({transform, mesh});
}

void GameObject::BaseUpdate()
{
    if(sprite != nullptr)
    {
        sprite->Update();
    }
    modelViewMatrix = this->camera->viewMatrix() * this->transform->modelMatrix();
    mesh->modelViewMatrix = this->modelViewMatrix;
}

void GameObject::Update()
{
    BaseUpdate();
}

void GameObject::BaseStart()
{
}

void GameObject::Start()
{
    BaseStart();
}

void GameObject::AssignSprite(float c, float r)
{

    sprite = new Sprite(mesh, c, r, material->texture->width, material->texture->height);
    sprite->Update();
}