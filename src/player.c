#include "player.h"
#include <math.h>

#define GRAVITY  0.012f
#define SPAWN    (Vector3){ -4, 12, 0 }

void UpdatePlayer(Player *p, float yaw, Map *map)
{
    float speed = 0.1f;
    float fx = -sinf(yaw);
    float fz = -cosf(yaw);

    if (IsKeyDown(KEY_W)) { p->position.x += speed * fx; p->position.z += speed * fz; }
    if (IsKeyDown(KEY_S)) { p->position.x -= speed * fx; p->position.z -= speed * fz; }
    if (IsKeyDown(KEY_D)) { p->position.x -= speed * fz; p->position.z += speed * fx; }
    if (IsKeyDown(KEY_A)) { p->position.x += speed * fz; p->position.z -= speed * fx; }

    p->velocity.y -= GRAVITY;
    p->position.y += p->velocity.y;

    int grounded = 0;
    for (int i = 0; i < map->count; i++) {
        GameObject t = map->tiles[i];
        float top  = t.position.y + t.size.y * 0.5f;
        int   inXZ = p->position.x >= t.position.x - t.size.x * 0.5f &&
                     p->position.x <= t.position.x + t.size.x * 0.5f &&
                     p->position.z >= t.position.z - t.size.z * 0.5f &&
                     p->position.z <= t.position.z + t.size.z * 0.5f;

        if (inXZ && p->position.y - 0.5f <= top && p->velocity.y <= 0) {
            p->position.y = top + 0.5f;
            p->velocity.y = 0.0f;
            grounded = 1;
        }
    }

    if (IsKeyDown(KEY_SPACE) && grounded)
        p->velocity.y = 0.2f;

    if (p->position.y < 0.0f) {
        p->position  = SPAWN;
        p->velocity.y = 0.0f;
    }
}

void DrawPlayer(Player p)
{
    DrawCube(p.position, 1, 1, 1, BLUE);
    DrawCubeWires(p.position, 1, 1, 1, BLACK);
}
