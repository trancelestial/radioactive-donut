#ifndef STATE_H
#define STATE_H

#include "player.h"
#include "shield.h"
#include "atom.h"
#include "debuffer.h"

extern float ModelView[16];

// global
extern int score, time_left;
extern char score_str[30];
extern int start;

// timer counters
extern float t_hover_mainr, t_rot_el;
extern unsigned int t_debuf;

// atoms
extern atom a[A_MAX_NUM];
extern int n_a;
extern int atom_gen_interval;

// shields
extern int n_s; //max allowed shields
extern int n_s_current; //current shields
extern shield s[S_MAX_NUM];
extern int shield_hit_ind;
extern int draw_hit_point_flag;

// player
extern float v_x, v_y, v_z;

// striking
extern int main_strike_interval;
extern int warning_delay_interval;
extern int strike_delay_interval;
extern int draw_ray_flag;
extern float main_rad_y;
extern float warning_done;
extern int atom_strike_n;//intex of atom being striked
extern int shield_duration;
extern int main_rad_boost;//indicator of radiation donut being boosted by debuffers

// debuffers
debuffer cr[CR_MAX_NUM];
int n_debuf;
int cr_duration;
int slow_active_num, range_active_num, dmg_active_num;//active debuffers



#endif