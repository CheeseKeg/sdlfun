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
