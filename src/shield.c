#include <GL/glut.h>
#include "shield.h"
#include "state.h"
#include "texture_handle.h"
#define M_PI 3.14159265358979323846

void draw_hit_point(float x, float y, float z, float r){
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  
  glBindTexture(GL_TEXTURE_2D, texture_names[2]);
  glBegin(GL_POLYGON);
  
    glTexCoord2f(0.35,0);
    glVertex3f(x-r,y,z-r);
    
    glTexCoord2f(1.35,0);
    glVertex3f(x+r,y,z-r);
    
    glTexCoord2f(1.35,1);
    glVertex3f(x+r,y,z+r);
    
    glTexCoord2f(0.35,1);
    glVertex3f(x-r,y,z+r);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_circle(float x, float y, float z, float r, int n){
  int vertices = n + 2;
  int i;
  float twoPI = 2.0f * M_PI;
  
  float circleVerticesX[vertices];
  float circleVerticesY[vertices];
  float circleVerticesZ[vertices];
  
  circleVerticesX[0] = x;
  circleVerticesY[0] = y;
  circleVerticesZ[0] = z;
  
  for (i = 1;i < vertices;i++ ){
    circleVerticesX[i] = x + ( r * cos( i *  twoPI/n ) );
    circleVerticesY[i] = y;
    circleVerticesZ[i] = z + ( r * sin( i * twoPI / n ) );
  }
  
  float allCircleVertices[vertices*3];
  
  for (i = 0;i < vertices;i++){
    allCircleVertices[i*3] = circleVerticesX[i];
    allCircleVertices[i*3+1] = circleVerticesY[i];
    allCircleVertices[i*3+2] = circleVerticesZ[i];
  }
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
  glDrawArrays(GL_TRIANGLE_FAN, 0, vertices);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_shield(shield *s, int n_s_current){
  int i;
  
  GLfloat ambient_coeffs[] = { 0.1745, 0.01175, 0.01175, 0.6 };
  GLfloat diffuse_coeffs[] = { 0.61424, 0.04136, 0.04136, 0.6 };
  GLfloat specular_coeffs[] = { 0.727811, 0.626959, 0.626959, 0.6 };
  
  glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffs);
  glMaterialf(GL_FRONT,  GL_SHININESS, 20);
  
  glNormal3f(0, 1, 0);
  
  glPushMatrix();
  for(i=0;i<n_s_current;i++){
    draw_circle(s[i].current.x, s[i].current.y, s[i].current.z, s[i].size, 50);

    if(draw_hit_point_flag && i == shield_hit_ind)
      draw_hit_point(s[i].hit_center.x, s[i].hit_center.y+0.001, s[i].hit_center.z, 1);
  }
  glPopMatrix();
}

int sort_compare(const void *a, const void *b){
  if((((shield*)a)->current).y > (((shield*)b)->current).y)
    return 1;
  else
    return -1;
}

void set_shield(float x, float y, float z) {
  s[n_s_current].current.x = x;
  s[n_s_current].current.y = y;
  s[n_s_current].current.z = z;
  s[n_s_current].size = 1;
  s[n_s_current].getting_hit = 0;
  s[n_s_current].num = n_s_current;

  n_s_current++;
  qsort(s, n_s_current, sizeof(shield), sort_compare); /* sort po y koordinati da bi iscrtavao shieldove dobrim redosledom zbog providnosti */
}