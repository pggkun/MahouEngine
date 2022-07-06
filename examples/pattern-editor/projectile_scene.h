#ifndef _PROJECTILE_SCENE_H_
#define _PROJECTILE_SCENE_H_
#include <memory_pool.h>
#include "projectile_object.h"

using std::list;

class ProjectileScene
{
private:
    list<ProjectileObject *>::iterator prox;
    list<ProjectileObject *>::iterator it;

public:
    ProjectileScene();
    ~ProjectileScene();

    void Add(ProjectileObject *entity);
    void Update();
    float scale = 0.25f;

    list<ProjectileObject *> entities;
    list<ProjectileObject *> inactive_entities;

    void Begin();
    ProjectileObject *Next();
    void Remove();

    ProjectileObject *GetFromInactive();

    MemoryPool memory_pool{2000, sizeof(ProjectileObject)};
};
#endif
