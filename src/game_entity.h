#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_
#include "transform.h"
#include "animated_sprite.h"
#include "camera.h"
#include "game_time.h"

class [[deprecated("Use GameObject or LuaObject instead.")]] GameEntity
{
    protected:
        glm::mat4 modelViewMatrix;
        int cr_count;       

    public:
        GameEntity(AnimatedSprite *spr, Camera *cam);
        ~GameEntity();

        Transform *transform;
        AnimatedSprite *sprite;
        Camera *camera;

        void BaseStart();
        void BaseUpdate();
        void Draw();
        virtual void Start();
        virtual void Update();
        void (GameEntity::*coroutine)() = NULL;
};

#endif