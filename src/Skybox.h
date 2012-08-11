#ifndef SKYBOX_H
#define SKYBOX_H

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <iostream>
#include "extra.h"

class Skybox
{
 public:

  Skybox();
  ~Skybox();

  void Init(const char* path);
  void Compile(float size);
  void Draw();
  void Kill();

 private:
  enum {SKY_LEFT = 0, SKY_BACK, SKY_RIGHT, SKY_FRONT, SKY_TOP, SKY_BOTTOM};
  unsigned int textures[6];

  int drawid;
};

#endif
