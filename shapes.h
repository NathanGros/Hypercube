#ifndef SHAPES_H
#define SHAPES_H

#include "graph.h"
#include "vectors.h"
#include <raylib.h>

graph4_t *make_cube(vector4_t origin, Color color);
graph4_t *make_pyramid(vector4_t origin, Color color);

#endif
