#ifndef DRAWING_H
#define DRAWING_H

#include <raylib.h>
#include "matrices.h"
#include "vectors.h"
#include "camera.h"

void init_draw();
void draw_point_3d(camera_t cam, vector3_t point, double radius, Color color);
void draw_line_3d(camera_t cam, vector3_t point1, vector3_t point2, double thickness, Color color);

#endif
