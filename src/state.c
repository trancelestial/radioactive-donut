#include "state.h"
#include "utils.h"
#include "config.h"

float ModelView[16];

// global
int score = 0, time_left = 100000;
char score_str[30];
int start = 0;

// timer
float t_hover_mainr = 0, t_rot_el=0;
unsigned int t_debuf = 0;

// atoms
atom a[A_MAX_NUM];
int n_a;
int atom_gen_interval;

// shields
int n_s = 5;
int n_s_current = 0;
shield s[S_MAX_NUM];
int shield_hit_ind = -1;
int draw_hit_point_flag = 0;

// player
float v_x, v_y, v_z;


// striking
int main_strike_interval = 6000;
int warning_delay_interval = 3000;
int strike_delay_interval = 4500;
int draw_ray_flag = 0;
float main_rad_y;
float warning_done = 0;
int atom_strike_n;
int shield_duration = 6000;
int main_rad_boost = 0;

// debuffers
debuffer cr[CR_MAX_NUM];
int n_debuf;
int cr_duration = 15000;
int slow_active_num = 0, range_active_num = 0, dmg_active_num = 0;//broji aktivne cripple radijacije
