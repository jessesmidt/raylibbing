#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Vector3 target;
    float   radius;
    float   yaw;
    float   pitch;
    bool    fly_mode;
    bool    pan_dragging;  /* true once right-drag exceeds threshold */
    /* fly mode state */
    Vector3 fly_pos;
    float   fly_yaw;
    float   fly_pitch;
} OrbitCamera;

void      orbit_camera_init(OrbitCamera *oc);
void      orbit_camera_update(OrbitCamera *oc);
Camera3D  orbit_to_camera3d(OrbitCamera *oc);

#endif
