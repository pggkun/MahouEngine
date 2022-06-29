#include "tile.h"

Tile::Tile(Material *mat, Camera *cam, glm::vec3 pos, Texture *tex, int x, int y, int r, int c)
{
    this->camera = cam;
    transform = new Transform();
   
    transform->position = pos;
    transform->rotationAxis = glm::vec3{0.0f, 0.0f, -1.0f};
    transform->angle = 0.0f;
    transform->scaleAmount = glm::vec3{1,1,1};

    this->mesh = new Mesh();
    float sx = (1.0 / c) * x;
    float ex = (1.0 / c) * (x + 1);
    float sy = (1.0 / r) * y;
    float ey = (1.0 / r) * (y + 1);
    float height = 1.0f;
    float width = ((tex->width / c) / (tex->height / r));
    this->mesh->SetupPlane(sx,
                     ex,
                     sy,
                     ey,
                     0,
                     0,
                     width,
                     height);
    this->mesh->Initialize();
    AssignMaterial(mat);
}

Tile::~Tile()
{
    RemoveMaterial();
    delete transform;
    delete sprite;
}

void Tile::CustomUpdate()
{
    modelViewMatrix = this->camera->viewMatrix() * this->transform->modelMatrix();
    this->mesh->modelViewMatrix = this->modelViewMatrix;
}

void Tile::RemoveMaterial()
{
    for (std::list<std::pair<Transform *, Mesh *>>::iterator it = material->objectsToDraw.begin(); it != material->objectsToDraw.end(); ++it)
    {
        if ((*it).second == mesh)
        {
            material->objectsToDraw.erase(it);
        }
    }
    material = nullptr;
}