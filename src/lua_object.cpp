#include "lua_object.h"
#include <stdio.h>

GameObjectLua :: GameObjectLua(char * file)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, file);
    lua_getglobal(L, "start");
    lua_call(L, 0, 0);

    lua_pushnumber(L, (double)transform->position.x);
    lua_setglobal(L, "x");

    lua_pushnumber(L, (double)transform->position.y);
    lua_setglobal(L, "y");
}

GameObjectLua ::~GameObjectLua()
{
    lua_close(L);
}

void GameObjectLua::Start(){}

void GameObjectLua::Update()
{
    lua_getglobal(L, "update");
    lua_call(L, 0, 0);

    lua_pushnumber(L, (double)GameTime::delta_time);
    lua_setglobal(L, "delta_time");

    float x, y;
    lua_getglobal(L, "x");
    if (lua_isnumber(L, -1))
    {
        x = (float)lua_tonumber(L, -1);
    }

    lua_getglobal(L, "y");
    if (lua_isnumber(L, -1))
    {
        y = (float)lua_tonumber(L, -1);
    }
    
    transform->position = glm::vec3{x,y, transform->position.z};
    BaseUpdate();
}