#include "extra.h"

void drawCube(float size)
{
  float size2 = size/2;

  glBegin(GL_QUADS);
  // Front
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(size2, size2, size2);
  glVertex3f(-size2, size2, size2);
  glVertex3f(-size2, -size2, size2);
  glVertex3f(size2, -size2, size2);
  // Back
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(size2, size2, -size2);
  glVertex3f(-size2, size2, -size2);
  glVertex3f(-size2, -size2, -size2);
  glVertex3f(size2, -size2, -size2);
  // Left
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(-size2, size2, size2);
  glVertex3f(-size2, size2, -size2);
  glVertex3f(-size2, -size2, -size2);
  glVertex3f(-size2, -size2, size2);
  // Right
  glColor3f(1.0, 1.0, 0.0);
  glVertex3f(size2, size2, size2);
  glVertex3f(size2, size2, -size2);
  glVertex3f(size2, -size2, -size2);
  glVertex3f(size2, -size2, size2);
  // Top
  glColor3f(1.0, 0.0, 1.0);
  glVertex3f(size2, size2, size2);
  glVertex3f(-size2, size2, size2);
  glVertex3f(-size2, size2, -size2);
  glVertex3f(size2, size2, -size2);
  // Bottom
  glColor3f(0.0, 1.0, 1.0);
  glVertex3f(size2, -size2, size2);
  glVertex3f(-size2, -size2, size2);
  glVertex3f(-size2, -size2, -size2);
  glVertex3f(size2, -size2, -size2);

  glEnd();

  return;
}

unsigned int loadTexture(const char* filename)
{
  SDL_Surface* image = SDL_LoadBMP(filename);

  unsigned int id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, image->pixels);

  SDL_FreeSurface(image);

  return id;
}

bool raysphere(float xc, float yc, float zc, float xd, float yd, float zd, float xs, float ys, float zs, float r)
{
  float b = 2*(xd*(xs-xc)+yd*(ys-yc)+zd*(zs-zc));
  float c = xs*xs-2*xs*xc+xc*xc+ys*ys-2*ys*yc+yc*yc+zs*zs-2*zs*zc+zc*zc-r*r;
  float discriminant = (b*b-4*c);

  bool intersecting = false;

  if (discriminant >= 0)
    intersecting = true;

  return intersecting;
}

bool rayplane(float nx,float ny,float nz,float xs,float ys,float zs,float xd,float yd,float zd,coordinate p1,coordinate p2,coordinate p3,coordinate p4)
{
        float a = xd*nx + yd*ny + zd*nz;
        if(a==0)
                return false;
        float t = ((p1.x*nx + p1.y*ny + p1.z*nz - nx*xs - ny*ys - nz*zs)/a);
        if(t<0)
                return false;
        float x = xs + t*xd;
        float y = ys + t*yd;
        float z = zs + t*zd;
        coordinate cp(x,y,z);
        if(abs(trianglearea(p1,p3,p4) - trianglearea(p1,p4,cp)-trianglearea(p1,p3,cp)-trianglearea(p3,p4,cp))<0.000001 || abs(trianglearea(p1,p2,p3)-trianglearea(p1,p2,cp)-trianglearea(p2,p3,cp)-trianglearea(p1,p3,cp))<0.000001)
                return true;
        return false;
}
 
 
float trianglearea(coordinate p1,coordinate p2,coordinate p3)
{
        // Heron formula
        float a=sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y)+(p2.z-p1.z)*(p2.z-p1.z));
        float b=sqrt((p3.x-p2.x)*(p3.x-p2.x)+(p3.y-p2.y)*(p3.y-p2.y)+(p3.z-p2.z)*(p3.z-p2.z));
        float c=sqrt((p1.x-p3.x)*(p1.x-p3.x)+(p1.y-p3.y)*(p1.y-p3.y)+(p1.z-p3.z)*(p1.z-p3.z));
        float s=(a+b+c)/2;
        return (sqrt(s*((s-a)*(s-b)*(s-c))));
}
