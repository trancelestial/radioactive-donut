#include "utils.h"
#include <GL/glut.h>

void normalize(vector *v)
{
    float i = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
    if(i!=0){
        v->x /= i;
        v->y /= i;
        v->z /= i;
    }
}

vector multiply(vector a, vector b)
{
    vector v;
    
    v.x=a.y*b.z - a.z*b.y;
    v.y=a.z*b.y - a.x*b.z;
    v.y=a.x*b.y - a.y*b.x;
    
    return v;
}

vector multiply_cord(float ax, float ay, float az, float bx, float by, float bz)
{
    vector v;
    
    v.x=ay*bz - az*by;
    v.y=az*bx - ax*bz;
    v.z=ax*by - ay*bx;
    
    return v;
}
