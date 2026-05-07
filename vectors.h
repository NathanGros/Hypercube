#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>

typedef struct {
	double w;
	double x;
	double y;
	double z;
} vector4_t;

typedef struct {
	double x;
	double y;
	double z;
} vector3_t;

typedef struct {
	double x;
	double y;
} vector2_t;

double vector4_norm(vector4_t v);
double vector3_norm(vector3_t v);
vector4_t vector4_add(vector4_t v1, vector4_t v2);
vector4_t vector4_scalar_multiply(vector4_t v, double n);

#endif
