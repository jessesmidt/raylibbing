#include "picking.h"
#include "voxel_grid.h"
#include <math.h>

PickResult dda_pick(Ray ray) {
    PickResult result = {0};

    float ox = ray.position.x, oy = ray.position.y, oz = ray.position.z;
    float dx = ray.direction.x, dy = ray.direction.y, dz = ray.direction.z;

    /* normalize — GetMouseRay should already do this, but be safe */
    float len = sqrtf(dx*dx + dy*dy + dz*dz);
    if (len < 1e-6f) return result;
    dx /= len; dy /= len; dz /= len;

    /* starting cell: voxel (ix,iy,iz) is centered at (ix,iy,iz),
       occupies [ix-0.5, ix+0.5] on each axis */
    int ix = (int)floorf(ox + 0.5f);
    int iy = (int)floorf(oy + 0.5f);
    int iz = (int)floorf(oz + 0.5f);

    /* step direction per axis */
    int sx = (dx >= 0) ? 1 : -1;
    int sy = (dy >= 0) ? 1 : -1;
    int sz = (dz >= 0) ? 1 : -1;

    /* how far along the ray between consecutive grid line crossings */
    float tdx = (fabsf(dx) > 1e-6f) ? fabsf(1.0f / dx) : 1e30f;
    float tdy = (fabsf(dy) > 1e-6f) ? fabsf(1.0f / dy) : 1e30f;
    float tdz = (fabsf(dz) > 1e-6f) ? fabsf(1.0f / dz) : 1e30f;

    /* distance to first boundary crossing on each axis:
       next boundary in +sx direction from cell ix is at ix + sx*0.5 */
    float tmx = (fabsf(dx) > 1e-6f) ? ((float)ix + sx * 0.5f - ox) / dx : 1e30f;
    float tmy = (fabsf(dy) > 1e-6f) ? ((float)iy + sy * 0.5f - oy) / dy : 1e30f;
    float tmz = (fabsf(dz) > 1e-6f) ? ((float)iz + sz * 0.5f - oz) / dz : 1e30f;

    /* face normal of the entry face (updated each step) */
    int face_x = 0, face_y = 0, face_z = 0;

    for (int step = 0; step < 128; step++) {
        if (ix >= 0 && ix < GRID_W &&
            iy >= 0 && iy < GRID_H &&
            iz >= 0 && iz < GRID_D) {
            if (VOXEL(ix, iy, iz).active) {
                result.hit = true;
                result.vx  = ix; result.vy  = iy; result.vz  = iz;
                result.nx  = face_x; result.ny  = face_y; result.nz  = face_z;
                return result;
            }
        }

        /* advance on the axis whose next boundary is closest */
        if (tmx <= tmy && tmx <= tmz) {
            ix    += sx;
            face_x = -sx; face_y = 0; face_z = 0;
            tmx   += tdx;
        } else if (tmy <= tmz) {
            iy    += sy;
            face_x = 0; face_y = -sy; face_z = 0;
            tmy   += tdy;
        } else {
            iz    += sz;
            face_x = 0; face_y = 0; face_z = -sz;
            tmz   += tdz;
        }
    }

    return result;
}
