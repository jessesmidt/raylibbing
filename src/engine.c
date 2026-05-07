#include "engine.h"
#include "player.h"
#include "map.h"
#include "scripting.h"
#include <math.h>

#define TICK_RATE  66
#define TICK_DT    (1.0f / TICK_RATE)

typedef struct {
    Camera3D camera;
    Map      map;
    Player   cube;
    float    yaw;
    float    pitch;
    float    accumulator;
    int      paused;
    int      quit;
} EngineState;

static EngineState state;

void engine_map_add_tile(float x, float y, float z, float sx, float sy, float sz)
{
    MapAdd(&state.map, (Vector3){x, y, z}, (Vector3){sx, sy, sz}, LIGHTGRAY);
}

void engine_set_player_spawn(float x, float y, float z)
{
    state.cube.position = (Vector3){x, y, z};
}

void init()
{
    state.camera.position   = (Vector3){ 0, 12, 6 };
    state.camera.target     = (Vector3){ 0, 11, 0 };
    state.camera.up         = (Vector3){ 0,  1, 0 };
    state.camera.fovy       = 60.0f;
    state.camera.projection = CAMERA_PERSPECTIVE;

    state.cube = (Player){
        .position = (Vector3){ 0, 12, 0 },
        .velocity = (Vector3){ 0,  0, 0 },
    };

    state.yaw   = 0.0f;
    state.pitch = 0.4f;

    InitWindow(800, 600, "dieding");
    SetTargetFPS(120);
    SetExitKey(KEY_NULL);   // disable raylib's default ESC-closes-window
    DisableCursor();

    scripting_init("scripts/main.lua");
}

void update()
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        state.paused = !state.paused;
        if (state.paused) EnableCursor();
        else              DisableCursor();
    }

    if (!state.paused) {
        // Mouse look runs every frame for responsiveness
        Vector2 delta = GetMouseDelta();
        state.yaw   -= delta.x * 0.003f;
        state.pitch += delta.y * 0.003f;
        if (state.pitch >  1.4f) state.pitch =  1.4f;
        if (state.pitch < -1.4f) state.pitch = -1.4f;

        // Physics at fixed 66 tick/s
        state.accumulator += GetFrameTime();
        while (state.accumulator >= TICK_DT) {
            UpdatePlayer(&state.cube, state.yaw, &state.map);
            state.accumulator -= TICK_DT;
        }

        float dist = 6.0f;
        state.camera.position = (Vector3){
            state.cube.position.x + dist * sinf(state.yaw) * cosf(state.pitch),
            state.cube.position.y + dist * sinf(state.pitch),
            state.cube.position.z + dist * cosf(state.yaw) * cosf(state.pitch)
        };
        state.camera.target = state.cube.position;
    }

    BeginDrawing();
    ClearBackground(GRAY);
    BeginMode3D(state.camera);
    DrawMap(&state.map);
    DrawPlayer(state.cube);
    EndMode3D();
    DrawFPS(10, 10);

    if (state.paused) {
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();

        DrawRectangle(0, 0, sw, sh, (Color){0, 0, 0, 120});
        DrawText("PAUSED", sw/2 - MeasureText("PAUSED", 40)/2, sh/2 - 80, 40, WHITE);

        Rectangle quit_btn = { sw/2 - 100, sh/2, 200, 50 };
        bool hovered = CheckCollisionPointRec(GetMousePosition(), quit_btn);
        DrawRectangleRec(quit_btn, hovered ? RED : DARKGRAY);
        DrawRectangleLinesEx(quit_btn, 2, WHITE);
        DrawText("Quit", quit_btn.x + quit_btn.width/2 - MeasureText("Quit", 20)/2,
                 quit_btn.y + 15, 20, WHITE);

        if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            state.quit = 1;
    }

    EndDrawing();
}

void shutdown()
{
    scripting_shutdown();
    CloseWindow();
}

int should_close()
{
    return WindowShouldClose() || state.quit;
}
