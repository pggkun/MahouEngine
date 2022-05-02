#ifndef _PROJECTILE_SCENE_H_
#define _PROJECTILE_SCENE_H_
#include <memory_pool.h>
#include "projectile_object.h"

using std::list;

class ProjectileScene
{
private:
    list<ProjectileObject *>::iterator next;
    list<ProjectileObject *>::iterator it;

public:
    ProjectileScene();
    ~ProjectileScene();

    void Add(ProjectileObject *entity);
    void Update();

    list<ProjectileObject *> entities;
    list<ProjectileObject *> inactive_entities;

    void Begin();
    ProjectileObject *Next();
    void Remove();

    ProjectileObject *GetFromInactive();

    MemoryPool memory_pool{1000, sizeof(ProjectileObject)};
};
#endif
