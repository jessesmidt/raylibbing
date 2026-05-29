#ifndef PICKING_H
#define PICKING_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    bool hit;
    int  vx, vy, vz;   /* hit voxel grid coords */
    int  nx, ny, nz;   /* outward face normal (-1, 0, or 1 per axis) */
} PickResult;

PickResult dda_pick(Ray ray);

#endif
