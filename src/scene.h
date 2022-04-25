#ifndef _SCENE_H_
#define _SCENE_H_
#include "game_entity.h"
#include <list>

using std::list;

class Scene
{
    private:
        
        list<GameEntity *>::iterator next;
        list<GameEntity *>::iterator it;

    public:
        Scene();
        ~Scene();

        void Add(GameEntity *entity);
        void Update();
        void Draw();

        list<GameEntity *> entities;
        
        void Begin();
        GameEntity *Next();
        void Remove();
};
#endif