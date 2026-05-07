#include "scripting.h"
#include "engine.h"
#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>
#include <lua5.4/lualib.h>
#include <stdio.h>

static lua_State *L;

static int l_map_add_tile(lua_State *L)
{
    float x  = luaL_checknumber(L, 1);
    float y  = luaL_checknumber(L, 2);
    float z  = luaL_checknumber(L, 3);
    float sx = luaL_checknumber(L, 4);
    float sy = luaL_checknumber(L, 5);
    float sz = luaL_checknumber(L, 6);
    engine_map_add_tile(x, y, z, sx, sy, sz);
    return 0;
}

static int l_set_player_spawn(lua_State *L)
{
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    float z = luaL_checknumber(L, 3);
    engine_set_player_spawn(x, y, z);
    return 0;
}

void scripting_init(const char *path)
{
    L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "map_add_tile",     l_map_add_tile);
    lua_register(L, "set_player_spawn", l_set_player_spawn);

    if (luaL_dofile(L, path) != LUA_OK) {
        fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

void scripting_shutdown()
{
    if (L) lua_close(L);
}
