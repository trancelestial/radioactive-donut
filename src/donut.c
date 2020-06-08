#include "donut.h"
#include "GL/glut.h"
#include "state.h"

void draw_main_rad(int boosted){
  
  GLfloat ambient_coeffs[] = { 0, 0.4, 0.3, 0.8 };
  GLfloat diffuse_coeffs[] = { boosted, 0.5, 0.3, 0.8 };
  GLfloat specular_coeffs[] = { 0.4, 0.4, 0.4, 0.8 };
  GLfloat shininess = 30;
  
  glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffs);
  glMaterialf(GL_FRONT,  GL_SHININESS, shininess);
  
  main_rad_y = MAINR_HEIGHT+sin(t_hover_mainr/4)/5;
  
  glPushMatrix();
    glTranslatef(0, main_rad_y, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(0.3,1,10,10);
  glPopMatrix();
}
