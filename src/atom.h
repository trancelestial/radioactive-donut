#ifndef ATOM_H
#define ATOM_H

#include "config.h"
#include "utils.h"

typedef struct{
    vector current, norm; /* relative distance to atom */
    float t; /* rotation angle*/
    float r; /* rotatin trajectory radius */
}electron;

typedef struct{
    vector current;
    int n_e;/* current number of electrons */
    int init_n_e;/* initial number of electrons, implicitly defines type */
    electron e[E_MAX_NUM];
    float r;/* atom radius */
}atom;

void init_atom();
void draw_atom(atom *a, int n_a);
void draw_ray(atom *a, int n_a, float rad_x, float rad_y, float rad_z);
atom generate_atom(void);

#endif