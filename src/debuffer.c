#include "debuffer.h"
#include "state.h"
#include "GL/glut.h"

void draw_debuffer(debuffer *cr, int n_debuf){
  int i, dy;
  glPushMatrix();
  for(i=0;i<n_debuf;i++){
    glPushMatrix();
      GLfloat ambient_coeffs[] = { 0.5, 0.5, 0.5, 1 };
      GLfloat diffuse_coeffs[] = { 0.6, 0.4, 0.6, 1 };
      GLfloat specular_coeffs[] = { 0.6, 0.4, 0.6, 1 };
      GLfloat shininess = 40;
      
      if(cr[i].type == CUT_RANGE){
	diffuse_coeffs[0] = 0.4;
	diffuse_coeffs[1] = 0.6;
	diffuse_coeffs[2] = 0.6;
      }
      else if(cr[i].type == BOOST_DMG){
	diffuse_coeffs[0] = 0.7;
	diffuse_coeffs[1] = 0.2;
	diffuse_coeffs[2] = 0.2;
      }
      
      glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_coeffs);
      glMaterialf(GL_FRONT,  GL_SHININESS, shininess);
      
      /* animacija rasta */
      if(cr[i].size < cr[i].goal_size)
	cr[i].size+=0.005;
      /* animacija penjanja na visinu CR_H */
      else if(!cr[i].on_path){
	dy = CR_H - cr[i].current.y;
	if(dy < 0.01)
	  cr[i].on_path = 1;
	cr[i].current.y+=cr[i].height_dist/100;
      }
      
      glTranslatef(cr[i].current.x, cr[i].current.y, cr[i].current.z);
      glScalef(cr[i].size, cr[i].size, cr[i].size);
      glRotatef(t_debuf%361, 0, 1, 0);
      
      glutSolidOctahedron();
    glPopMatrix();
  }
  glPopMatrix();
}

debuffer generate_debuf(float x, float y, float z){
  debuffer c;
  
  c.current.x = x;
  c.current.y = y;
  c.current.z = z;
  
  c.duration = (unsigned)rand()%10000+7000;
  c.strike_interval = (unsigned)rand()%(c.duration/2)+2000;
  c.type = (unsigned)rand()%3;
  c.goal_size = 0.5;
  c.size = 0.01;
  
  c.height_dist = CR_H - y;
  
  c.on_path = 0;
  
  return c;
}

