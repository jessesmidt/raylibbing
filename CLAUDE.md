# dieding0.01

Raylib-based 3D platformer engine with an embedded Lua scripting layer.

## Architecture

**C owns everything real-time:** rendering, physics, collision, input, audio. Anything that runs every frame stays in C.

**Lua owns everything scripted:** level definitions, dialogue, cutscenes, game state machines, menus, save/load, entity spawning.

Lua calls into C to tell the engine what to put in the world — not to run the world. The only interface between Lua and C is `include/engine.h`. Lua is embedded directly in the C engine via `lua_State` — no external process, no shared library boundary.

When new engine features are needed that Lua should control, add a function to `engine.h` and register it with the Lua VM.

## Next session — Lua migration

Replacing the Python scripting layer with embedded Lua (Lua C API). Plan:

1. Add Lua as a dependency (system package or bundled)
2. Create `src/scripting.c` + `include/scripting.h` — owns `lua_State`, init/shutdown, and exposes engine functions to Lua
3. Register C functions (`map_add_tile`, `set_player_spawn`, etc.) with the Lua VM
4. Replace `scripts/test.py` with `scripts/main.lua`
5. Update Makefile to link `-llua`

## Project structure

```
src/
  engine.c      — game loop, camera, EngineState
  player.c      — movement, gravity, collision, jump
  map.c         — tile storage and draw loop
  scripting.c   — lua_State, C function registration (next session)
include/
  engine.h      — public API
  player.h      — Player struct + functions
  map.h         — Map/GameObject structs + functions
  scripting.h   — scripting init/shutdown (next session)
scripts/
  main.lua      — Lua entry point (replaces test.py, next session)
build/
  libengine.so  (may become a plain executable once Lua is embedded)
```

## Build

```
make        # builds build/libengine.so
make run    # runs scripts/test.py (temporary, until Lua migration)
make clean  # removes build/
```

## Conventions

- Map tiles added via `MapAdd()` — automatically get collision and draw calls, no extra wiring needed.
- `SPAWN` and `GRAVITY` are defined in `player.c`.
- Camera is third-person orbit controlled by mouse (yaw/pitch). WASD moves relative to camera yaw.
- Void threshold is `y < 0` — falling below respawns the player at `SPAWN`.
