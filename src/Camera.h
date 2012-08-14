#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "objloader.h"

class Camera
{
 public:
  float x, y, z, yaw, pitch, lockangle;

  Camera(float, float, float, float, float);
  ~Camera();

  void Move(float, float);
  void MoveUp(float, float);
  void MoveTo(coordinate position);
  void Lock();
  void Control(float, float, bool, int, int);
  void Update();
};

#endif
