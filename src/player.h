#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"
#include <GL/glut.h>

typedef struct{
  vector current, past, lookat;
  float mov_v, h_max, cube_size, rot_ud, rot_lr, max_range;
  int collides;
  material_coeffs mat;
}player;

extern player p;

void init_player();
void draw_player(player p);
void player_move();

#endif