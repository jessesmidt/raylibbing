#ifndef ENGINE_H
#define ENGINE_H

// Game loop
void init();
void update();
void shutdown();
int  should_close();

// Lua-callable API
void engine_map_add_tile(float x, float y, float z, float sx, float sy, float sz);
void engine_set_player_spawn(float x, float y, float z);

#endif
