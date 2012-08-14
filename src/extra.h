#ifndef EXTRA_H
#define EXTRA_H

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

struct coordinate
{
  float x, y, z;

  coordinate(float a, float b, float c);
};

void drawCube(float size);
unsigned int loadTexture(const char* filename);

bool raysphere(float xc, float yc, float zc, float xd, float yd, float zd, float xs, float ys, float zs, float r);
bool rayplane(float nx, float ny, float nz, float xs, float ys, float zs, float xd, float yd, float zd, coordinate p1, coordinate p2, coordinate p3, coordinate p4, float* dist, coordinate* point);
bool sphereplane(coordinate& sp, coordinate vn, coordinate p1, coordinate p2, coordinate p3, coordinate p4, float r);

float trianglearea(coordinate p1, coordinate p2, coordinate p3);

#endif
