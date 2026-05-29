#include "voxel_grid.h"
#include "raylib.h"

Voxel grid[GRID_W * GRID_H * GRID_D];

void grid_init(void) {
    /* hardcode a small cluster to verify indexing and rendering */
    VOXEL(4, 0, 4).active = 1; VOXEL(4, 0, 4).r = 200; VOXEL(4, 0, 4).g =  80; VOXEL(4, 0, 4).b =  80;
    VOXEL(5, 0, 4).active = 1; VOXEL(5, 0, 4).r = 200; VOXEL(5, 0, 4).g =  80; VOXEL(5, 0, 4).b =  80;
    VOXEL(6, 0, 4).active = 1; VOXEL(6, 0, 4).r = 200; VOXEL(6, 0, 4).g =  80; VOXEL(6, 0, 4).b =  80;
    VOXEL(5, 1, 4).active = 1; VOXEL(5, 1, 4).r =  80; VOXEL(5, 1, 4).g = 200; VOXEL(5, 1, 4).b =  80;
    VOXEL(5, 2, 4).active = 1; VOXEL(5, 2, 4).r =  80; VOXEL(5, 2, 4).g = 200; VOXEL(5, 2, 4).b =  80;
    VOXEL(4, 0, 5).active = 1; VOXEL(4, 0, 5).r =  80; VOXEL(4, 0, 5).g =  80; VOXEL(4, 0, 5).b = 200;
    VOXEL(5, 0, 5).active = 1; VOXEL(5, 0, 5).r = 200; VOXEL(5, 0, 5).g = 200; VOXEL(5, 0, 5).b =  80;
    VOXEL(6, 0, 5).active = 1; VOXEL(6, 0, 5).r =  80; VOXEL(6, 0, 5).g = 200; VOXEL(6, 0, 5).b = 200;
}

void grid_draw_floor(void) {
    Color col = (Color){ 70, 70, 70, 255 };
    /* lines along Z, one per X boundary: x = -0.5, 0.5, 1.5, ..., 31.5 */
    for (int x = 0; x <= GRID_W; x++) {
        float wx = (float)x - 0.5f;
        DrawLine3D((Vector3){ wx, 0.0f, -0.5f },
                   (Vector3){ wx, 0.0f, (float)GRID_D - 0.5f }, col);
    }
    /* lines along X, one per Z boundary */
    for (int z = 0; z <= GRID_D; z++) {
        float wz = (float)z - 0.5f;
        DrawLine3D((Vector3){ -0.5f,           0.0f, wz },
                   (Vector3){ (float)GRID_W - 0.5f, 0.0f, wz }, col);
    }
}

void grid_draw(void) {
    for (int z = 0; z < GRID_D; z++) {
        for (int y = 0; y < GRID_H; y++) {
            for (int x = 0; x < GRID_W; x++) {
                Voxel *v = &VOXEL(x, y, z);
                if (!v->active) continue;
                Vector3 pos = { (float)x, (float)y, (float)z };
                Color   col = { v->r, v->g, v->b, 255 };
                DrawCube(pos, 1.0f, 1.0f, 1.0f, col);
                DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, (Color){0,0,0,80});
            }
        }
    }
}
