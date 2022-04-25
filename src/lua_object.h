#ifndef _GAME_OBJECT_LUA_H_
#define _GAME_OBJECT_LUA_H_
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include "game_object.h"

class GameObjectLua : public GameObject
{
    protected:
        lua_State *L;
    public:
        GameObjectLua(char *file);
        ~GameObjectLua();
        void Update() override;
        void Start() override;
};

#endif