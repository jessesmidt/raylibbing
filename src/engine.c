#include "raylib.h"
#include "engine.h"

typedef struct {
    Vector3 position;
    Vector3 size;
    Color color;
} GameObject;

void DrawGameObject(GameObject obj) {
    DrawCube(obj.position, obj.size.x, obj.size.y, obj.size.z, obj.color);
}

typedef struct {
    Camera3D camera;
    GameObject floor;
} EngineState;

static EngineState state;

void init() {
    state.camera.position = (Vector3){ 0, 2, 6 };
    state.camera.target   = (Vector3){ 0, 1, 0 };
    state.camera.up       = (Vector3){ 0, 1, 0 };
    state.camera.fovy     = 60.0f;
    state.camera.projection = CAMERA_PERSPECTIVE;

    state.floor = (GameObject){
        .position = (Vector3){0, 0, 0},
        .size     = (Vector3){10, 1, 10},
        .color    = LIGHTGRAY
    };

    InitWindow(800, 600, "raylib + python");
}

void update(EngineState *e)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("hello from C", 200, 200, 20, BLACK);
    BeginMode3D(state.camera);
    DrawGameObject(state.floor);

    EndMode3D();
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
