#ifndef UTILS_H
#define UTILS_H

#include <math.h>

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

typedef struct{
  float x, y, z;
}vector;

typedef struct{
  float r, g, b, a;
}rgba;

typedef struct{
  rgba ambient, diffuse, specular;
}material_coeffs;

void normalize(vector* v);
vector multiply(vector a, vector b);
vector multiply_cord(float ax, float ay, float az, float bx, float by, float bz);

#endif