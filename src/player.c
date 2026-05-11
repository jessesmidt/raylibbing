#include "player.h"
#include <math.h>

#define GRAVITY  0.012f
#define SPAWN    (Vector3){ -4, 12, 0 }

void UpdatePlayer(Player *p, float yaw, Map *map)
{
    // --- Movement ---
    // Convert camera yaw angle into a forward direction vector (fx, fz).
    // sinf/cosf give us the XZ components of the direction the camera faces.
    // WASD moves relative to that direction so controls always match the view.
    float speed = 0.1f;
    float fx = -sinf(yaw);
    float fz = -cosf(yaw);

    if (IsKeyDown(KEY_W)) { p->position.x += speed * fx; p->position.z += speed * fz; }
    if (IsKeyDown(KEY_S)) { p->position.x -= speed * fx; p->position.z -= speed * fz; }
    if (IsKeyDown(KEY_D)) { p->position.x -= speed * fz; p->position.z += speed * fx; }
    if (IsKeyDown(KEY_A)) { p->position.x += speed * fz; p->position.z -= speed * fx; }

    // --- Gravity ---
    // Accumulate downward velocity each tick, then apply it to Y position.
    p->velocity.y -= GRAVITY;
    p->position.y += p->velocity.y;

    // --- Collision ---
    // AABB min-overlap resolution: for each solid tile, compute how far the
    // player overlaps it on each axis (X, Y, Z).
    //
    // fabsf(dx) is the absolute (always positive) distance between centers on
    // that axis. Subtracting it from the sum of half-extents gives the overlap —
    // positive means penetrating, zero or negative means no contact on that axis.
    //
    // We only have a real collision if ALL three axes overlap at once.
    // We then push the player out along whichever axis has the smallest overlap
    // (minimum separation vector), since that's the shortest path out.
    int grounded = 0;
    for (int i = 0; i < map->count; i++) {
        GameObject *t = &map->tiles[i];
        if (!t->solid) continue;

        // Distance between player center and tile center on each axis
        float dx = p->position.x - t->position.x;
        float dy = p->position.y - t->position.y;
        float dz = p->position.z - t->position.z;

        // Overlap = combined half-extents minus the center-to-center distance.
        // Player is a 1x1x1 cube so its half-extents are all 0.5.
        float ox = (0.5f + t->size.x * 0.5f) - fabsf(dx);
        float oy = (0.5f + t->size.y * 0.5f) - fabsf(dy);
        float oz = (0.5f + t->size.z * 0.5f) - fabsf(dz);

        // Any axis with no overlap means no collision — skip this tile
        if (ox <= 0 || oy <= 0 || oz <= 0) continue;

        // Push out on the smallest overlap axis.
        // The sign of dx/dy/dz tells us which direction to push.
        // Before pushing back on X or Z, try a step-up: if the tile's top
        // surface is within 0.5 units above the player's feet, lift the player
        // onto it instead of blocking them — this makes stairs walkable.
        float tile_top = t->position.y + t->size.y * 0.5f;
        float step     = tile_top - (p->position.y - 0.5f);
        int   can_step = (step > 0.0f && step <= 0.5f);

        if (ox < oy && ox < oz) {
            if (can_step) { p->position.y = tile_top + 0.5f; grounded = 1; }
            else          { p->position.x += (dx >= 0 ? ox : -ox); }
        } else if (oz < oy) {
            if (can_step) { p->position.y = tile_top + 0.5f; grounded = 1; }
            else          { p->position.z += (dz >= 0 ? oz : -oz); }
        } else {
            // Y axis: dy >= 0 means player center is above tile center (landed on top)
            if (dy >= 0) {
                p->position.y += oy;
                if (p->velocity.y <= 0) { p->velocity.y = 0.0f; grounded = 1; }
            } else {
                // Player center is below tile center (bumped head on underside)
                p->position.y -= oy;
                if (p->velocity.y > 0) p->velocity.y = 0.0f;
            }
        }
    }

    // --- Jump ---
    if (IsKeyDown(KEY_SPACE) && grounded)
        p->velocity.y = 0.2f;

    // --- Void respawn ---
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
