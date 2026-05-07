#include "engine.h"
#include "player.h"
#include "map.h"
#include <math.h>

typedef struct {
    Camera3D camera;
    Map      map;
    Player   cube;
    float    yaw;
    float    pitch;
} EngineState;

static EngineState state;

void init()
{
    state.camera.position   = (Vector3){ 0, 12, 6 };
    state.camera.target     = (Vector3){ 0, 11, 0 };
    state.camera.up         = (Vector3){ 0,  1, 0 };
    state.camera.fovy       = 60.0f;
    state.camera.projection = CAMERA_PERSPECTIVE;

    MapAdd(&state.map, (Vector3){  0, 10, 0 }, (Vector3){ 10, 1, 10 }, LIGHTGRAY);
    MapAdd(&state.map, (Vector3){ 12, 10, 0 }, (Vector3){ 10, 1, 10 }, LIGHTGRAY);

    state.cube = (Player){
        .position = (Vector3){ -4, 12, 0 },
        .velocity = (Vector3){  0,  0, 0 },
    };

    state.yaw   = 0.0f;
    state.pitch = 0.4f;

    InitWindow(800, 600, "raylib + python");
    DisableCursor();
}

void update()
{
    UpdatePlayer(&state.cube, state.yaw, &state.map);

    Vector2 delta = GetMouseDelta();
    state.yaw   -= delta.x * 0.003f;
    state.pitch += delta.y * 0.003f;
    if (state.pitch >  1.4f) state.pitch =  1.4f;
    if (state.pitch < -1.4f) state.pitch = -1.4f;

    float dist = 6.0f;
    state.camera.position = (Vector3){
        state.cube.position.x + dist * sinf(state.yaw) * cosf(state.pitch),
        state.cube.position.y + dist * sinf(state.pitch),
        state.cube.position.z + dist * cosf(state.yaw) * cosf(state.pitch)
    };
    state.camera.target = state.cube.position;

    BeginDrawing();
    ClearBackground(GRAY);
    BeginMode3D(state.camera);
    DrawMap(&state.map);
    DrawPlayer(state.cube);
    EndMode3D();
    DrawFPS(10, 10);
    EndDrawing();
}

void shutdown()
{
    CloseWindow();
}

int should_close()
{
    return WindowShouldClose();
}
