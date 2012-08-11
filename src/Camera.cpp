#include "Camera.h"

#ifndef M_PI
#define M_PI = 3.1415926535
#endif

Camera::Camera(float _x, float _y, float _z, float _pitch, float _yaw)
{
  x = _x;
  y = _y;
  z = _z;
  pitch = _pitch;
  yaw = _yaw;

  lockangle = 75.0;
}

void Camera::Lock()
{
  if (pitch > lockangle)
    pitch = lockangle;
  else if (pitch < -lockangle)
    pitch = -lockangle;

  if (yaw < 0.0)
    yaw += 360.0;
  if (yaw >= 360.0)
    yaw -= 360.0;
}

void Camera::Move(float distance, float direction)
{
  float radian = (yaw + direction)*M_PI/180.0;

  x += sin(radian)*distance;
  z += cos(radian)*distance;

  return;
}

void Camera::MoveUp(float distance, float direction)
{
  float radian = (pitch + direction)*M_PI/180.0;

  y -= sin(radian)*distance;

  return;
}
void Camera::Control(float movevelosity, float mousevelosity, bool mousein, int screenx, int screeny)
{
  if (mousein)
  {
    int middlex = screenx/2;
    int middley = screeny/2;

    int mousestatex, mousestatey;

    SDL_ShowCursor(SDL_DISABLE);

    SDL_GetMouseState(&mousestatex, &mousestatey);
    yaw += (middlex - mousestatex)*mousevelosity;
    pitch += (middley - mousestatey)*mousevelosity;

    Lock();

    SDL_WarpMouse(middlex, middley);

    Uint8* state = SDL_GetKeyState(NULL);
    if (state[SDLK_w])
    {
      if (pitch != 90 && pitch != -90)
	Move(mousevelosity, 0.0);
      MoveUp(mousevelosity, 0.0);
    }
    else if (state[SDLK_s])
    {
      if (pitch != 90 && pitch != -90)
	Move(mousevelosity, 180.0);
      MoveUp(mousevelosity, 180.0);
    }
    if (state[SDLK_a])
      Move(mousevelosity, 90.0);
    else if (state[SDLK_d])
      Move(mousevelosity, 270.0);
  }
  else
  {
    SDL_ShowCursor(SDL_ENABLE);
  }

  glRotatef(-pitch, 1.0, 0.0, 0.0);
  glRotatef(-yaw, 0.0, 1.0, 0.0);
}

void Camera::Update()
{
  glTranslatef(x, y, z);
}
