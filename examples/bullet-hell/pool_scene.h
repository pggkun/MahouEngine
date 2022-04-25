#ifndef _POOL_SCENE_H_
#define _POOL_SCENE_H_
#include <scene.h>
#include <memory_pool.h>
#include "projectile.h"

using std::list;

class PoolScene
{
    private:
        list<Projectile *>::iterator next;
        list<Projectile *>::iterator it;

    public:
        PoolScene();
        ~PoolScene();

        void Add(Projectile *entity);
        void Update();
        void Draw();

        list<Projectile *> entities;
        list<Projectile *> inactive_entities;

        void Begin();
        Projectile *Next();
        void Remove();

        Projectile *GetFromInactive();

        MemoryPool memory_pool{5000, sizeof(Projectile)};
};
#endif



