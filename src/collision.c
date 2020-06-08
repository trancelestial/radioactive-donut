#include "collision.h"
#include "state.h"
#include "utils.h"

int collision(atom *a, int n_a, player *p){
  for(int i=0;i<n_a;i++)
    if(collision_single(a[i], p))
      return 1;
    
  return 0;
}

/* AABB - sphere */
int collision_single(atom a, player *p){
  float x, y, z, dist;
  
  x = max(p->current.x - p->cube_size/2, min(a.current.x, p->current.x + p->cube_size/2));
  y = max(p->current.y - p->cube_size/2, min(a.current.y, p->current.y + p->cube_size/2));
  z = max(p->current.z - p->cube_size/2, min(a.current.z, p->current.z + p->cube_size/2));
  
  dist = sqrt((x-a.current.x)*(x-a.current.x) + (y-a.current.y)*(y-a.current.y) + (z-a.current.z)*(z-a.current.z));
  
  if(dist <= a.r)
    return 1;
  
  return 0;
}

int collision_rad_shield(shield *s, int n_s_current, float rad_x, float rad_y, float rad_z, atom a){
  int i;/*, hit = 0;*/
  float ts, x, z;// y = 0 
  
  for(i=n_s_current-1;i>=0/* && !hit*/;i--){
    /* trazimo ts(parametar) na kojoj se "visini seku ravan stita(n=(0,1,0) i P-centar stita)" */
    ts = (s[i].current.y-a.current.y)/(rad_y-a.current.y);
    
    /* ubacujemo ts u jednacinu prave zraka i trazimo gde se nalazi presecna tacka ravni stita i prave, zatim proveravamo
      da li se ta tacka nalazi u stitu(tj. u krugu koji predstavlja stit)*/
    x = a.current.x + ts*(rad_x-a.current.x);
    z = a.current.z + ts*(rad_z-a.current.z);
    
    if(sqrt((x-s[i].current.x)*(x-s[i].current.x) + (z-s[i].current.z)*(z-s[i].current.z)) <= s[i].size){
      s[i].hit_center.x = x;
      s[i].hit_center.y = s[i].current.y;
      s[i].hit_center.z = z;
       return i;
    }
  } 
  return -1;
}