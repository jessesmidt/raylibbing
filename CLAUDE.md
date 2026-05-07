# dieding0.01

Raylib-based 3D platformer engine with embedded Lua scripting. Builds as a standalone executable.

## Architecture

**C owns everything real-time:** rendering, physics, collision, input, audio. Anything that runs every frame stays in C.

**Lua owns everything scripted:** level definitions, dialogue, cutscenes, game state machines, menus, save/load, entity spawning.

Lua is embedded in the C engine via `lua_State`. The script runs once at startup to configure the world. The only surface Lua touches is the functions registered in `src/scripting.c`.

When new engine features need to be Lua-controllable, implement the logic in C, expose it via `engine.h`, and register a wrapper in `scripting.c`.

## Project structure

```
src/
  main.c        — entry point, runs the game loop
  engine.c      — EngineState, init/update/shutdown, Lua-callable API impl
  player.c      — movement, gravity, collision, jump
  map.c         — tile storage and draw loop
  scripting.c   — lua_State, registers C functions for Lua
include/
  engine.h      — game loop + Lua-callable API declarations
  player.h      — Player struct + functions
  map.h         — Map/GameObject structs + functions
  scripting.h   — scripting init/shutdown
scripts/
  main.lua      — level definition (tiles, spawn point)
build/
  dieding       — compiled executable
```

## Build & run

```
sudo apt install liblua5.4-dev lua5.4   # one-time setup
make        # builds build/dieding
make run    # runs build/dieding
make clean  # removes build/
```

## Adding a new tile

In `scripts/main.lua`:
```lua
map_add_tile(x, y, z, size_x, size_y, size_z)
```

## Adding a new Lua-callable function

1. Implement logic in `engine.c`, declare in `engine.h`
2. Add a `static int l_yourfunc(lua_State *L)` wrapper in `scripting.c`
3. Register it: `lua_register(L, "your_func", l_yourfunc);`
4. Call it from `scripts/main.lua`

## Conventions

- `SPAWN` and `GRAVITY` are defined in `player.c`.
- Camera is third-person orbit controlled by mouse (yaw/pitch). WASD moves relative to camera yaw.
- Void threshold is `y < 0` — falling below respawns the player at spawn.
- Tile colors are currently hardcoded to LIGHTGRAY in `engine_map_add_tile` — add a color param when needed.
