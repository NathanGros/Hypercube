#ifndef DRAWING_H
#define DRAWING_H

#include <raylib.h>
#include "graph.h"
#include "matrices.h"
#include "vectors.h"
#include "camera.h"

void init_draw();
void draw_graph_4d(camera_t cam, graph4_t *graph);

#endif
