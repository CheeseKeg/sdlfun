#include "Skybox.h"

Skybox::Skybox()
{
  drawid = -1;
}

Skybox::~Skybox() {}

void Skybox::Init(const char* path)
{
  std::string filenames[6] = {"west.bmp", "south.bmp", "east.bmp", "north.bmp", "up.bmp", "down.bmp"};
  std::cout << "Initializing sky box" << std::endl;

  for (int i = 0; i < 6; i++)
  {
    filenames[i] = path + filenames[i];
    textures[i] = loadTexture(filenames[i].c_str(), true);
  }

  return;
}

void Skybox::Compile(float size)
{
  float size2 = size/2;

  drawid = glGenLists(1);
  glNewList(drawid, GL_COMPILE);

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);

  // Back
  glBindTexture(GL_TEXTURE_2D, textures[SKY_BACK]);

  glBegin(GL_QUADS);

  glTexCoord2f(0, 0);
  glVertex3f(size2, size2, size2);
  glTexCoord2f(1, 0);
  glVertex3f(-size2, size2, size2);
  glTexCoord2f(1, 1);
  glVertex3f(-size2, -size2, size2);
  glTexCoord2f(0, 1);
  glVertex3f(size2, -size2, size2);

  glEnd();

  // Front
  glBindTexture(GL_TEXTURE_2D, textures[SKY_FRONT]);

  glBegin(GL_QUADS);

  glTexCoord2f(1, 0);
  glVertex3f(size2, size2, -size2);
  glTexCoord2f(0, 0);
  glVertex3f(-size2, size2, -size2);
  glTexCoord2f(0, 1);
  glVertex3f(-size2, -size2, -size2);
  glTexCoord2f(1, 1);
  glVertex3f(size2, -size2, -size2);

  glEnd();

  // Left
  glBindTexture(GL_TEXTURE_2D, textures[SKY_LEFT]);

  glBegin(GL_QUADS);

  glTexCoord2f(0, 0);
  glVertex3f(-size2, size2, size2);
  glTexCoord2f(1, 0);
  glVertex3f(-size2, size2, -size2);
  glTexCoord2f(1, 1);
  glVertex3f(-size2, -size2, -size2);
  glTexCoord2f(0, 1);
  glVertex3f(-size2, -size2, size2);

  glEnd();

  // Right
  glBindTexture(GL_TEXTURE_2D, textures[SKY_RIGHT]);

  glBegin(GL_QUADS);

  glTexCoord2f(1, 0);
  glVertex3f(size2, size2, size2);
  glTexCoord2f(0, 0);
  glVertex3f(size2, size2, -size2);
  glTexCoord2f(0, 1);
  glVertex3f(size2, -size2, -size2);
  glTexCoord2f(1, 1);
  glVertex3f(size2, -size2, size2);

  glEnd();

  // Top
  glBindTexture(GL_TEXTURE_2D, textures[SKY_TOP]);

  glBegin(GL_QUADS);

  glTexCoord2f(1, 0);
  glVertex3f(size2, size2, size2);
  glTexCoord2f(0, 0);
  glVertex3f(-size2, size2, size2);
  glTexCoord2f(0, 1);
  glVertex3f(-size2, size2, -size2);
  glTexCoord2f(1, 1);
  glVertex3f(size2, size2, -size2);

  glEnd();

  // Bottom
  glBindTexture(GL_TEXTURE_2D, textures[SKY_BOTTOM]);

  glBegin(GL_QUADS);

  glTexCoord2f(1, 1);
  glVertex3f(size2, -size2, size2);
  glTexCoord2f(0, 1);
  glVertex3f(-size2, -size2, size2);
  glTexCoord2f(0, 0);
  glVertex3f(-size2, -size2, -size2);
  glTexCoord2f(1, 0);
  glVertex3f(size2, -size2, -size2);

  glEnd();

  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);

  glEndList();

  return;
}

void Skybox::Draw()
{
  if (drawid != 0)
    glCallList(drawid);

  return;
}

void Skybox::Kill()
{
  glDeleteTextures(6, &textures[0]);

  return;
}
