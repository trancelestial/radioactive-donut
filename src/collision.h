#ifndef COLLISION_H
#define COLLISION_H

#include "player.h"
#include "atom.h"
#include "shield.h"


int collision(atom *a, int n_a, player *p);
int collision_single(atom a, player *p);
int collision_rad_shield(shield *s, int n_s_current, float rad_x, float rad_y, float rad_z, atom a);


#endif