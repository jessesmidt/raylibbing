#ifndef MAP_H
#define MAP_H

#include "raylib.h"

#define MAX_TILES 64

typedef struct {
    Vector3 position;
    Vector3 size;
    Color   color;
} GameObject;

typedef struct {
    GameObject tiles[MAX_TILES];
    int        count;
} Map;

void MapAdd(Map *m, Vector3 pos, Vector3 size, Color color);
void DrawMap(Map *m);

#endif
