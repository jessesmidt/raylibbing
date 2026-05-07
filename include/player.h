#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "map.h"

typedef struct {
    Vector3 position;
    Vector3 velocity;
    Color   color;
} Player;

void UpdatePlayer(Player *p, float yaw, Map *map);
void DrawPlayer(Player p);

#endif
