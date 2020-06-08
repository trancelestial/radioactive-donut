#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<GL/glut.h>

#include "image.h"
#include "config.h"
#include "utils.h"
#include "texture_handle.h"

#include "player.h"
#include "shield.h"
#include "atom.h"
#include "debuffer.h"
#include "donut.h"
#include "state.h"
#include "scene.h"
#include "collision.h"


/* callbacks */
static void on_display(void);
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_keyboard_up(unsigned char key, int x, int y);
static void on_timer(int value);
static void on_mouse_move(int x, int y);
static void on_mouse(int button, int state, int x, int y);


int main(int argc, char **argv){
  /* init GLUT */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
   
  /* init gl window*/
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  glutInitWindowPosition(150, 50);
  glutCreateWindow("Atom defender");
  
    /* register callbacks */
  glutDisplayFunc(on_display);
  glutReshapeFunc(on_reshape);
  glutKeyboardFunc(on_keyboard);
  glutKeyboardUpFunc(on_keyboard_up);
  glutPassiveMotionFunc(on_mouse_move);
  glutMouseFunc(on_mouse);
   
  /* init OpenGL*/
  glClearColor(0, 0, 0, 0);
  glEnable(GL_DEPTH_TEST);
  glLineWidth(1.5);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  /* init lighting */
  GLfloat light_position[] = { 1, 10, 1, 0.0 }; 

  GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1 };
  GLfloat light_diffuse[] = { 1, 1, 1, 1 };
  GLfloat light_specular[] = { 1, 1, 1, 1 };
  
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(1.5, 5, 16, 0, 0, 0, 0, 1, 0);
  
  glShadeModel(GL_SMOOTH);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  
  init_textures();

  init_player();
  init_atom();
 
  time_t t;
  srand((unsigned)time(&t));
   
  glutMainLoop();
   
  return 0;
}

static void on_display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(1.5, 5, 16, 0, 0, 0, 0, 1, 0);
  
  draw_atom(a, n_a);
    
  draw_main_rad(main_rad_boost);
  
  draw_player(p);
  
  if(draw_ray_flag && n_a > 0){
    draw_ray(a, n_a, 0, main_rad_y, 0);
  }

  draw_score(score_str);
  
  draw_debuffer(cr, n_debuf);
  
  draw_norm_plane();
  
  draw_shield(s, n_s_current);

  glutSwapBuffers();
}

static void on_timer(int value){
  /* game ended */
  if(start == -1)
    return;
  
  /* player movement */
  if(value == PLAYER_MOVE_ID){
    player_move();
    
    glutTimerFunc(PLAYER_MOVE_INTERVAL, on_timer, PLAYER_MOVE_ID);
  }
  /* atom generation */
  else if(value == ATOM_GEN_ID && n_a < A_MAX_NUM){
    if(n_a == 0)
      glutTimerFunc(main_strike_interval, on_timer, MAIN_STRIKE_ID);
    
    a[n_a++] = generate_atom();
    atom_gen_interval = 7000;
    
    glutTimerFunc(atom_gen_interval, on_timer, ATOM_GEN_ID);
  }
  /* choose atom to be struck */
  else if(value == MAIN_STRIKE_ID && n_a > 0){
    draw_ray_flag = 1;
    atom_strike_n = (int)rand()%n_a;
    
    glutTimerFunc(warning_delay_interval, on_timer, WARNING_DELAY_ID);
    glutTimerFunc(strike_delay_interval, on_timer, STRIKE_DELAY_ID);
  }
  /* strike atom */
  else if(value == WARNING_DELAY_ID){
    warning_done = 1;
    shield_hit_ind = collision_rad_shield(s, n_s_current, 0, main_rad_y, 0, a[atom_strike_n]);
    if(shield_hit_ind >= 0)
      s[shield_hit_ind].getting_hit = 1;
  }
  /* end striking animation */
  else if(value == STRIKE_DELAY_ID){
    draw_ray_flag = 0;
    warning_done = 0;
    
    /* no shield to defend */
    if(shield_hit_ind < 0){
      /* strike and pushe electron from atom */
      if(a[atom_strike_n].n_e > 1){
        a[atom_strike_n].n_e--;
        score-=3;
      }
      /* kill atom (no electrons left) */
      else {
        float x=a[atom_strike_n].current.x, y=a[atom_strike_n].current.y, z=a[atom_strike_n].current.z;/* set debuffer position */
        switch(a[atom_strike_n].init_n_e){
          case 1:
            score-=4;
          break;
          case 2:
            score-=8;
          break;
          case 3:
            score-=12;
        }

        for(int i = atom_strike_n; i < n_a-1; i++)
          a[i] = a[i+1];
        if(n_a == A_MAX_NUM){
          n_a--;
          glutTimerFunc(atom_gen_interval, on_timer, ATOM_GEN_ID);
        }
        else
          n_a--;
        /* generate new debuff */
        cr[n_debuf] = generate_debuf(x, y, z);
	
        /* add debuffed sign to player */
        if(cr[n_debuf].type == SLOW){
          p.mov_v/=2;
          p.mat.diffuse.a = 0.2;
          p.mat.ambient.a = 0.2;
          p.mat.specular.a = 0.2;
          slow_active_num++;
        }
        else if(cr[n_debuf].type == CUT_RANGE){
          p.max_range*=0.3;
          if(fabs(p.lookat.x)>p.max_range)
          p.lookat.x = -p.max_range;
          range_active_num++;
          p.mat.diffuse.r = 0.9;
          p.mat.diffuse.b = 0.3;
        }
        else {
          main_strike_interval/=2;
          dmg_active_num++;
          main_rad_boost = 1;
        }
	
        n_debuf++;
        glutTimerFunc(cr_duration, on_timer, CR_DURATION_ID);
      }
      sprintf(score_str, "Score: %d", score);
    } else /* shield hit */
      s[shield_hit_ind].getting_hit = 0;
      shield_hit_ind = -1;
    
      glutTimerFunc(main_strike_interval, on_timer, MAIN_STRIKE_ID);
  }
  /* shield duration */
  else if(value == SHIELD_DURATION_ID){
    /* remove from shield queue */
    int i, j;
    for(i=0;i<n_s_current && s[i].num!=0;i++)
      s[i].num--;
    n_s_current--;
    for(j=i;j<n_s_current;j++){
      s[j]=s[j+1];
      s[j].num--;
    }
  }
  /* duration of debuff logic */
  else if(value == CR_DURATION_ID){
    int i;
    if(cr[0].type == SLOW){
      p.mov_v*=2;
      slow_active_num--;
      if(!slow_active_num){
        p.mat.diffuse.a = 1;
        p.mat.ambient.a = 1;
        p.mat.specular.a = 1;
      }
    }
    else if(cr[0].type == CUT_RANGE){
      p.max_range/=0.3;
      range_active_num--;
      if(!range_active_num){
        p.mat.diffuse.r = 0.3;
        p.mat.diffuse.b = 0.9;
      }
    }
    else{
      main_strike_interval*=2;
      dmg_active_num--;
      if(!dmg_active_num)
        main_rad_boost = 0;
    }
    
    n_debuf--;
    for(i=0;i<n_debuf;i++)
      cr[i]=cr[i+1];
  }
  
  glutPostRedisplay();
   
  t_hover_mainr += 0.5;
  t_rot_el += 0.05;
  t_debuf++;
}

static void on_mouse(int button, int state, int x, int y){
  /* aim on mouse scroll */
  if(fabs(p.lookat.x) < p.max_range && p.lookat.x < -0.2){
    if(button == 3)
      p.lookat.x-=0.1;
    else if(button == 4)
      p.lookat.x+=0.1;
  }
  else if(fabs(p.lookat.x) >= p.max_range)
    p.lookat.x = -p.max_range+0.01;
  else
    p.lookat.x = -0.201;
  
  /* put shield */
  if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
    if(n_s_current < n_s){
      set_shield(p.lookat.x*ModelView[0] + p.lookat.y*ModelView[4] + p.lookat.z*ModelView[8] + ModelView[12],
                 p.lookat.x*ModelView[1] + p.lookat.y*ModelView[5] + p.lookat.z*ModelView[9] + ModelView[13],
                 p.lookat.x*ModelView[2] + p.lookat.y*ModelView[6] + p.lookat.z*ModelView[10] + ModelView[14]);

      glutTimerFunc(shield_duration, on_timer, SHIELD_DURATION_ID);
    }
  }
  
  glutPostRedisplay();
}

static void on_mouse_move(int x, int y){
  p.rot_ud = 180*(SCREEN_HEIGHT-y)/SCREEN_HEIGHT+90;
  p.rot_lr = 2*360*x/SCREEN_WIDTH;
}

static void on_keyboard(unsigned char key, int x, int y){
  switch (key) {
    case 27:
      exit(0);
      break;

    case 'w':
    case 'W':
      v_z = -1;
      break;

    case 's':
    case 'S':
      v_z = 1;
      break;
	  
    case 'd':
    case 'D':
      v_x = 1;
      break;

    case 'a':
    case 'A':
      v_x = -1;
      break;
    
    case 'p':
    case 'P':
      start = -1;
      break;
      
    case 32:
      v_y = 1;
      break;
  }
  
  if(!start){
    start=1;
    glutWarpPointer(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    
    glutTimerFunc(PLAYER_MOVE_INTERVAL, on_timer, PLAYER_MOVE_ID);
    glutTimerFunc(atom_gen_interval, on_timer, ATOM_GEN_ID);
  }
}

static void on_keyboard_up(unsigned char key, int x, int y){
  switch(key) {
    case 'w':
    case 'W':
      v_z = 0;
      break;

    case 's':
    case 'S':
      v_z = 0;
      break;
	
    case 'd':
    case 'D':
      v_x = 0;
      break;

    case 'a':
    case 'A':
      v_x = 0;
      break;
      
    case 32:
      v_y = -1;
      break;
  }
}

static void on_reshape(int width, int height){
  glViewport(0, 0, width, height);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (float) width / height, 1, 500);
}