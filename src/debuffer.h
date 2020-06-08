#ifndef DEBUFFER_H
#define DEBUFFER_H

#include "utils.h"

typedef enum {SLOW, CUT_RANGE, BOOST_DMG} cripple_type;
typedef struct{
  vector current;
  int duration, strike_interval;
  cripple_type type;
  float goal_size, size;
  float height_dist; /* rastojanje u trenutku stvaranja po y od CR_ROT_H, koristim za racunajne brzine */
  int on_path; /* da li je dosao na kruznu putanju */
}debuffer; /* nastaju svaki put kad se unisti atom */

void draw_debuffer(debuffer *cr, int n_debuf);
debuffer generate_debuf(float x, float y, float z);

#endif