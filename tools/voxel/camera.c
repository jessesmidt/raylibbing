#include "camera.h"
#include "raymath.h"
#include <math.h>

#define ORBIT_SPEED      0.004f
#define PAN_SPEED        0.012f
#define ZOOM_FACTOR      0.08f   /* proportional: 8% of current radius per tick */
#define ZOOM_MIN         2.0f
#define ZOOM_MAX         80.0f
#define PITCH_MIN       -1.50f
#define PITCH_MAX        1.50f
#define FLY_SPEED        8.0f
#define FLY_LOOK_SPEED   0.003f

void orbit_camera_init(OrbitCamera *oc) {
    oc->target    = (Vector3){ 8.0f, 1.0f, 8.0f };
    oc->radius    = 24.0f;
    oc->yaw       = 0.7f;
    oc->pitch     = 0.4f;
    oc->fly_mode  = false;
    oc->fly_pos   = (Vector3){ 8.0f, 8.0f, 30.0f };
    oc->fly_yaw   = 0.0f;
    oc->fly_pitch = 0.0f;
}

void orbit_camera_update(OrbitCamera *oc) {
    float dt = GetFrameTime();

    /* toggle fly mode — compute orbit eye BEFORE flipping the flag */
    if (IsKeyPressed(KEY_F)) {
        if (!oc->fly_mode) {
            Camera3D tmp = orbit_to_camera3d(oc); /* fly_mode still false here */
            oc->fly_pos   = tmp.position;
            oc->fly_yaw   = oc->yaw + (float)PI;
            oc->fly_pitch = -oc->pitch;
            oc->fly_mode  = true;
        } else {
            oc->fly_mode = false;
        }
    }

    if (oc->fly_mode) {
        /* middle mouse → look */
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            Vector2 delta = GetMouseDelta();
            oc->fly_yaw   -= delta.x * FLY_LOOK_SPEED;
            oc->fly_pitch -= delta.y * FLY_LOOK_SPEED;
            if (oc->fly_pitch >  PITCH_MAX) oc->fly_pitch =  PITCH_MAX;
            if (oc->fly_pitch < -PITCH_MAX) oc->fly_pitch = -PITCH_MAX;
        }

        float cy = cosf(oc->fly_yaw), sy = sinf(oc->fly_yaw);
        float cp = cosf(oc->fly_pitch);
        Vector3 fwd   = { sy * cp, sinf(oc->fly_pitch), cy * cp };
        Vector3 right = { cy, 0.0f, -sy };
        float speed = FLY_SPEED * dt;

        if (IsKeyDown(KEY_W)) { oc->fly_pos = Vector3Add(oc->fly_pos, Vector3Scale(fwd,    speed)); }
        if (IsKeyDown(KEY_S)) { oc->fly_pos = Vector3Add(oc->fly_pos, Vector3Scale(fwd,   -speed)); }
        if (IsKeyDown(KEY_A)) { oc->fly_pos = Vector3Add(oc->fly_pos, Vector3Scale(right,  speed)); }  /* fixed */
        if (IsKeyDown(KEY_D)) { oc->fly_pos = Vector3Add(oc->fly_pos, Vector3Scale(right, -speed)); }  /* fixed */
        if (IsKeyDown(KEY_E)) { oc->fly_pos.y += speed; }
        if (IsKeyDown(KEY_Q)) { oc->fly_pos.y -= speed; }
        return;
    }

    /* --- orbit mode --- */

    /* middle drag → orbit */
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        Vector2 delta = GetMouseDelta();
        oc->yaw   += delta.x * ORBIT_SPEED;
        oc->pitch -= delta.y * ORBIT_SPEED;
        if (oc->pitch > PITCH_MAX) oc->pitch = PITCH_MAX;
        if (oc->pitch < PITCH_MIN) oc->pitch = PITCH_MIN;
    }

    /* scroll → zoom proportional to current radius */
    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f) {
        oc->radius *= (1.0f - wheel * ZOOM_FACTOR);
        if (oc->radius < ZOOM_MIN) oc->radius = ZOOM_MIN;
        if (oc->radius > ZOOM_MAX) oc->radius = ZOOM_MAX;
    }

    /* right drag → pan target; track drag vs click for picking */
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        oc->pan_dragging = false;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 delta = GetMouseDelta();
        if (fabsf(delta.x) + fabsf(delta.y) > 2.0f) oc->pan_dragging = true;
        if (oc->pan_dragging) {
            float pan = PAN_SPEED * oc->radius * 0.05f;
            Vector3 right = { cosf(oc->yaw), 0.0f, -sinf(oc->yaw) };
            Vector3 up    = { 0.0f, 1.0f, 0.0f };
            oc->target = Vector3Add(oc->target, Vector3Scale(right, -delta.x * pan));
            oc->target = Vector3Add(oc->target, Vector3Scale(up,     delta.y * pan));
        }
    }
}

Camera3D orbit_to_camera3d(OrbitCamera *oc) {
    Camera3D cam = {0};
    cam.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
    cam.fovy       = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    if (oc->fly_mode) {
        cam.position = oc->fly_pos;
        float cy = cosf(oc->fly_yaw), sy = sinf(oc->fly_yaw);
        float cp = cosf(oc->fly_pitch);
        Vector3 fwd = { sy * cp, sinf(oc->fly_pitch), cy * cp };
        cam.target = Vector3Add(oc->fly_pos, fwd);
    } else {
        float cp = cosf(oc->pitch);
        Vector3 offset = {
            sinf(oc->yaw) * cp * oc->radius,
            sinf(oc->pitch)    * oc->radius,
            cosf(oc->yaw) * cp * oc->radius
        };
        cam.position = Vector3Add(oc->target, offset);
        cam.target   = oc->target;
    }
    return cam;
}
