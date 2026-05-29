#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H

#include <stdint.h>

#define GRID_W 32
#define GRID_H 32
#define GRID_D 32

typedef struct {
    uint8_t       active;
    unsigned char r, g, b;
} Voxel;

extern Voxel grid[GRID_W * GRID_H * GRID_D];

#define VOXEL(x,y,z) grid[(z)*GRID_W*GRID_H + (y)*GRID_W + (x)]

void grid_init(void);
void grid_draw(void);
void grid_draw_floor(void);

#endif
