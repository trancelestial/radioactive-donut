#include "player.h"
#include "state.h"

player p;

void init_player()
{
  p = (player){
      {0, .25, 0}, {0, .25, 0},{-1,0,0},
      .05, 5, .5, 0, 0, 4, 0,
      {{0.01,0,0.4,1},{0.3,0.3,0.9,1},{0.4,0.4,0.7,1}}
  };
}
void draw_player(player p){
  GLfloat ambient_coeffs[] = { p.mat.ambient.r, p.mat.ambient.g, p.mat.ambient.b, p.mat.ambient.a };
  GLfloat diffuse_coeffs[] = { p.mat.diffuse.r, p.mat.diffuse.g, p.mat.diffuse.b, p.mat.diffuse.a };
  GLfloat specular_coeffs[] = { p.mat.specular.r, p.mat.specular.g, p.mat.specular.b, p.mat.specular.a };
  GLfloat shininess = 50;
  
  glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffs);
  glMaterialf(GL_FRONT,  GL_SHININESS, shininess);
  
  glPushMatrix();
  glPushMatrix();
  glLoadIdentity();
    glTranslatef(p.current.x, p.current.y, p.current.z);
    glRotatef(p.rot_lr, 0, 1, 0);
    //vector v = multiply_cord(p.lookat.x, p.lookat.y, p.lookat.z, 1.0, 0.0, 0.0);
    //printf("%f -- %f -- %f\n", v.x, v.y, v.z);
    glRotatef(p.rot_ud, 0, 0, -1);
    
    glGetFloatv(GL_MODELVIEW_MATRIX, ModelView);
  glPopMatrix();
  
  glTranslatef(p.current.x, p.current.y, p.current.z);
  glRotatef(p.rot_lr, 0, 1, 0);
  glRotatef(p.rot_ud, 0, 0, -1);
  
  glutSolidCube(p.cube_size);

  glLineWidth(1.5);
  /* nisan */
  ambient_coeffs[0]=1;
  diffuse_coeffs[0]=1;
  
  glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffs);
  glMaterialf(GL_FRONT,  GL_SHININESS, shininess);
  
  glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(p.lookat.x, p.lookat.y, p.lookat.z);
  glEnd();

  glPopMatrix();
}

void player_move()
{
  p.past.x=p.current.x;
  p.past.y=p.current.y;
  p.past.z=p.current.z;
  
  p.current.x=v_x;
  p.current.y=v_y;
  p.current.z=v_z;
  
  normalize(&p.current);
  
  p.current.x=p.current.x*p.mov_v + p.past.x;
  p.current.y=p.current.y*p.mov_v + p.past.y;
  p.current.z=p.current.z*p.mov_v + p.past.z;
  
  if(p.current.z + p.cube_size/2 > TERRAIN_WIDTH) p.current.z = TERRAIN_WIDTH - p.cube_size/2;
  if(p.current.z - p.cube_size/2 < -TERRAIN_WIDTH) p.current.z = -TERRAIN_WIDTH + p.cube_size/2;
  if(p.current.x + p.cube_size/2 > TERRAIN_WIDTH) p.current.x = TERRAIN_WIDTH - p.cube_size/2;
  if(p.current.x - p.cube_size/2 < -TERRAIN_WIDTH) p.current.x = -TERRAIN_WIDTH + p.cube_size/2;
  if(p.current.y < p.cube_size/2){p.current.y = p.cube_size/2; v_y = 0;}
  if(p.current.y >= p.h_max){p.current.y = p.h_max; v_y = 0;}
  
  p.collides = collision(a, n_a, &p)?1:0;
  
  if(p.collides){
    p.current.x = p.past.x;
    p.current.y = p.past.y;
    p.current.z = p.past.z;
    v_y=0;
  }else if(p.current.y > p.cube_size/2 && v_y != 1)
    v_y = -1;
  
  time_left -= PLAYER_MOVE_INTERVAL;
  if(time_left <= 0)
    start = -1;
}



