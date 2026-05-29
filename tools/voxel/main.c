#include "raylib.h"
#include "voxel_grid.h"
#include "camera.h"
#include "picking.h"

#define DEFAULT_R 200
#define DEFAULT_G 200
#define DEFAULT_B 200

int main(void) {
    InitWindow(1280, 720, "Voxel Tool - Phase 3");
    SetTargetFPS(60);

    grid_init();

    OrbitCamera oc;
    orbit_camera_init(&oc);

    while (!WindowShouldClose()) {
        orbit_camera_update(&oc);
        Camera3D cam = orbit_to_camera3d(&oc);

        Ray       ray  = GetMouseRay(GetMousePosition(), cam);
        PickResult pick = dda_pick(ray);

        /* place voxel on left click */
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && pick.hit) {
            int px = pick.vx + pick.nx;
            int py = pick.vy + pick.ny;
            int pz = pick.vz + pick.nz;
            if (px >= 0 && px < GRID_W &&
                py >= 0 && py < GRID_H &&
                pz >= 0 && pz < GRID_D) {
                VOXEL(px, py, pz).active = 1;
                VOXEL(px, py, pz).r = DEFAULT_R;
                VOXEL(px, py, pz).g = DEFAULT_G;
                VOXEL(px, py, pz).b = DEFAULT_B;
            }
        }

        /* remove voxel on right click (not drag) */
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && !oc.pan_dragging && pick.hit) {
            VOXEL(pick.vx, pick.vy, pick.vz).active = 0;
        }

        BeginDrawing();
            ClearBackground((Color){30, 30, 30, 255});
            BeginMode3D(cam);
                grid_draw();
                grid_draw_floor();

                if (pick.hit) {
                    Vector3 hpos = { (float)pick.vx, (float)pick.vy, (float)pick.vz };
                    /* highlight hovered voxel */
                    DrawCubeWires(hpos, 1.02f, 1.02f, 1.02f, YELLOW);

                    /* show placement preview */
                    int px = pick.vx + pick.nx;
                    int py = pick.vy + pick.ny;
                    int pz = pick.vz + pick.nz;
                    if (px >= 0 && px < GRID_W &&
                        py >= 0 && py < GRID_H &&
                        pz >= 0 && pz < GRID_D &&
                        !VOXEL(px, py, pz).active) {
                        Vector3 ppos = { (float)px, (float)py, (float)pz };
                        DrawCubeWires(ppos, 1.02f, 1.02f, 1.02f, GREEN);
                    }
                }
            EndMode3D();

            DrawText("Phase 3: Picking", 10, 10, 20, RAYWHITE);
            if (oc.fly_mode)
                DrawText("FLY  (F to toggle) | LClick: place  RClick: remove", 10, 34, 16, YELLOW);
            else
                DrawText("Mid: orbit  Scroll: zoom  RDrag: pan  F: fly | LClick: place  RClick: remove", 10, 34, 16, GRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
