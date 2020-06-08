#include "atom.h"
#include <GL/glut.h>
#include <stdio.h>
#include "texture_handle.h"
#include "state.h"
#include "collision.h"

GLUquadricObj *sphere = NULL;

void init_atom()
{
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
}

void draw_atom(atom *a, int n_a){
  glPushMatrix();
  for(int i = 0;i < n_a;i++)
  {
    glPushMatrix();
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      glTranslatef(a[i].current.x, a[i].current.y, a[i].current.z);
      glPushMatrix();
        glRotatef((t_debuf/a[i].n_e)%361, 0, 1, 0);
        glRotatef(90, 1, 0, 0);
        glBindTexture(GL_TEXTURE_2D, texture_names[0]);
        glEnable(GL_COLOR_MATERIAL);
        glColor4f(1,1,1,1);
	
        gluSphere(sphere, a[i].r, 20, 20);
      glPopMatrix();
      
      glBindTexture(GL_TEXTURE_2D, 0);
      glDisable(GL_COLOR_MATERIAL);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      
      GLfloat ambient_coeffs[] ={ 0.0, 0.0, 0.6, 1};
      GLfloat diffuse_coeffs[]  ={ 0, 0, 0.6, 1 };
      GLfloat specular_coeffs[] ={ 0.2, 0.2, 0.2, 1 };
      GLfloat shininess = 20;
      
      glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffs);
      glMaterialf(GL_FRONT,  GL_SHININESS, shininess);
      
      for(int j = 0;j < a[i].n_e;j++)
      {
        glPushMatrix();	  
          glRotatef(360 * t_rot_el/a[i].e[j].t, a[i].e[j].norm.x, a[i].e[j].norm.y, a[i].e[j].norm.z);
          glTranslatef(a[i].e[j].current.x, a[i].e[j].current.y, a[i].e[j].current.z);
          glutSolidSphere(.1, 5, 5);
        glPopMatrix();
      }
    glPopMatrix();
  }
  glPopMatrix();
}

atom generate_atom(){
  int i;
  float random_var = (float)rand()/(float)(RAND_MAX)*6;
  atom a;

  if(random_var <= 1.3) {
    a.n_e = a.init_n_e = 3;
    score+=7;
  }
  else if(random_var <= 3){
    a.n_e = a.init_n_e = 2;
    score+=5;
  }
  else{
    a.n_e = a.init_n_e = 1;
    score+=3;
  }
  
  /* grow radius of atom based on num of electrons*/
  a.r = ATOM_MAX_RADIUS/3+(2*ATOM_MAX_RADIUS)/(3+2*(E_MAX_NUM-a.n_e));
  //a.r = ((float)rand()/(float)(RAND_MAX))*ATOM_MAX_RADIUS/2+ATOM_MAX_RADIUS/(2+E_MAX_NUM-a.n_e);

  a.current.y = a.r;
  a.current.x = ((float)rand()/(float)(RAND_MAX))*(TERRAIN_WIDTH-a.r)*2-(TERRAIN_WIDTH-a.r);
  a.current.z = ((float)rand()/(float)(RAND_MAX))*(TERRAIN_WIDTH-a.r)*2-(TERRAIN_WIDTH-a.r);
  
  while(collision_single(a, &p)){
    a.current.x = ((float)rand()/(float)(RAND_MAX))*(TERRAIN_WIDTH-a.r)*2-(TERRAIN_WIDTH-a.r);
    a.current.z = ((float)rand()/(float)(RAND_MAX))*(TERRAIN_WIDTH-a.r)*2-(TERRAIN_WIDTH-a.r);
  }
  
  for(i = 0;i < a.n_e;i++){
    a.e[i].r = a.r + ((float)rand()/(float)(RAND_MAX));
    a.e[i].t = ((float)rand()/(float)(RAND_MAX))*10+25;
    a.e[i].current.x = 0;
    a.e[i].current.y = a.e[i].r;
    a.e[i].current.z = 0;
    a.e[i].norm.x = ((float)rand()/(float)(RAND_MAX))*2-1;
    a.e[i].norm.y = ((float)rand()/(float)(RAND_MAX))*2-1;
    a.e[i].norm.z = ((float)rand()/(float)(RAND_MAX))*2-1;
  }
  
  sprintf(score_str, "Score: %d", score);
  
  return a;
}

void draw_ray(atom *a, int n_a, float rad_x, float rad_y, float rad_z){
  int i;
  glLineWidth(1.5);
  draw_hit_point_flag = 0;
  
  glEnable(GL_COLOR_MATERIAL);
  glColor4f(0,1,0,0.6);
  
  glPushMatrix();
  if(!warning_done) {/* draw warning line*/
    glBegin(GL_LINES);
      glVertex3f(0, main_rad_y, 0);
      if(shield_hit_ind >= 0) {
        for(i=0;i<n_s_current;i++) // prevent bug with generating shild same time as ray is drawn
          if(s[i].getting_hit)
            shield_hit_ind = i;

        glVertex3f(s[shield_hit_ind].hit_center.x, s[shield_hit_ind].hit_center.y, s[shield_hit_ind].hit_center.z);
      } else
    glVertex3f(a[atom_strike_n].current.x, a[atom_strike_n].current.y, a[atom_strike_n].current.z);
    glEnd();   
  } else {/* draw ray */
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBindTexture(GL_TEXTURE_2D, texture_names[1]);
    glBegin(GL_POLYGON);
      glNormal3f(0,1,0);
      
      glTexCoord2f(0, 0);
      glVertex3f(-0.1, main_rad_y, 0);
      
      glTexCoord2f(1, 0);
      glVertex3f(0.1, main_rad_y, 0);
      
      if(shield_hit_ind >= 0){
        for(i=0;i<n_s_current;i++) 
          if(s[i].getting_hit){
            shield_hit_ind = i;
            draw_hit_point_flag = 1;
          }
        glTexCoord2f(1, 1);
        glVertex3f(s[shield_hit_ind].hit_center.x+0.1, s[shield_hit_ind].hit_center.y, s[shield_hit_ind].hit_center.z);
        glTexCoord2f(0, 1);
        glVertex3f(s[shield_hit_ind].hit_center.x-0.1, s[shield_hit_ind].hit_center.y, s[shield_hit_ind].hit_center.z);
      } else {
        glTexCoord2f(1, 1);
        glVertex3f(a[atom_strike_n].current.x+0.1, a[atom_strike_n].current.y, a[atom_strike_n].current.z);
        glTexCoord2f(0, 1);
        glVertex3f(a[atom_strike_n].current.x-0.1, a[atom_strike_n].current.y, a[atom_strike_n].current.z);
      }
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  }
  glPopMatrix();
  glDisable(GL_COLOR_MATERIAL);
}
