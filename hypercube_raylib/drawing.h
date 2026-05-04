#ifndef DRAWING_H
#define DRAWING_H

#include <raylib.h>
#include "matrices.h"
#include "vectors.h"
#include "camera.h"

void init_draw();
void draw_point_4d(camera_t cam, vector4_t point, double radius, Color color);
void draw_line_4d(camera_t cam, vector4_t point1, vector4_t point2, double thickness, Color color);
void draw_cube_4d(camera_t cam, vector4_t cube[16], Color color);

#endif
