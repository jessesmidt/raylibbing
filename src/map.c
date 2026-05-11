#include "map.h"

void MapAdd(Map *m, Vector3 pos, Vector3 size, Color color, int solid)
{
    if (m->count >= MAX_TILES) return;
    m->tiles[m->count++] = (GameObject){ pos, size, color, solid };
}

void DrawMap(Map *m)
{
    for (int i = 0; i < m->count; i++) {
        GameObject t = m->tiles[i];
        DrawCube(t.position, t.size.x, t.size.y, t.size.z, t.color);
        DrawCubeWires(t.position, t.size.x, t.size.y, t.size.z, BLACK);
    }
}
