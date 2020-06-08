#include "scene.h"
#include "config.h"
#include <GL/glut.h>

void draw_norm_plane(){
  GLfloat ambient_coeffs[] = { 0.05,0.05,0.05,0.2 };
  GLfloat diffuse_coeffs[] = { 0.5,0.5,0.5,0.2};
  GLfloat specular_coeffs[] = { 0.7,0.7,0.7,0.2};
  GLfloat shininess = 15;
  glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffs);
  glMaterialf(GL_FRONT,  GL_SHININESS, shininess);

  glBegin(GL_POLYGON);
    glNormal3f(0,1,0);
    glVertex3f(-TERRAIN_WIDTH, 0, TERRAIN_WIDTH);
    glVertex3f(TERRAIN_WIDTH, 0, TERRAIN_WIDTH);
    glVertex3f(TERRAIN_WIDTH, 0, -TERRAIN_WIDTH);
    glVertex3f(-TERRAIN_WIDTH, 0, -TERRAIN_WIDTH);
  glEnd();
}


void draw_score(char *score) {
  glEnable(GL_COLOR_MATERIAL);
  glPushMatrix();
    glColor4f(1,1,1,1);
    glRasterPos3f(-11, -5, 0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, score);
  glPopMatrix();
  glDisable(GL_COLOR_MATERIAL);
}

