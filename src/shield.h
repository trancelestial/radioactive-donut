#ifndef SHIELD_H
#define SHIELD_H

#include "utils.h"

typedef struct{
  vector current, hit_center/* mesto gde je pogodila radijacija */;
  float size;
  int getting_hit;/* indikator da li se atom trenutno gadja radijacijom */
  int num; /* "redni broj" */
}shield;

void draw_shield(shield *s, int n_s_current);
void draw_hit_point(float x, float y, float z, float r);
void draw_circle(float x, float y, float z, float r, int n);
void set_shield(float x, float y, float z);
int sort_compare(const void *a, const void *b);

#endif