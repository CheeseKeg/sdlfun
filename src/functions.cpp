#include "functions.h"

coordinate::coordinate(float a, float b, float c)
{
  x = a;
  y = b;
  z = c;
}

face::face(int facen, int f1, int f2, int f3)
{
  facenum = facen;
  faces[0] = f1;
  faces[1] = f2;
  faces[2] = f3;
  four = false;
}

face::face(int facen, int f1, int f2, int f3, int f4)
{
  facenum = facen;
  faces[0] = f1;
  faces[1] = f2;
  faces[2] = f3;
  faces[3] = f4;
  four = true;
}

int loadObject(const char* filename)
{
  std::vector<std::string*> coord;
  std::vector<coordinate*> vertex;
  std::vector<face*> faces;
  std::vector<coordinate*> normals;

  std::ifstream in(filename);
  if(!in.is_open())
  {
    std::cout << "Unable to open object file: " << filename << std::endl;
    return -1;
  }

  char buf[256];
  while (!in.eof())
  {
    in.getline(buf, 256);
    coord.push_back(new std::string(buf));
  }
  for (unsigned int i = 0; i < coord.size(); i++)
  {
    char operation = (*coord[i])[0];
    if (operation == '#')
      // Comment
      continue;
    else if (operation == 'v')
    {
      char operation2 = (*coord[i])[1];
      if (operation2 == ' ')
      {
	// Vertex
	float tmpx, tmpy, tmpz;
	sscanf(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz);
	vertex.push_back(new coordinate(tmpx, tmpy, tmpz));
      }
      else if (operation2 == 'n')
      {
	// Vector normal
	float tmpx, tmpy, tmpz;
        sscanf(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
	normals.push_back(new coordinate(tmpx, tmpy, tmpz));
      }
    }
    else if (operation == 'f')
    {
      // Face
      int a, b, c, d, e;
      if (count(coord[i]->begin(), coord[i]->end(), ' ') == 4)
      {
	sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b, &e, &b);
	faces.push_back(new face(b, a, c, d, e));
      }
      else
      {
	sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b);
	faces.push_back(new face(b, a, c, d));
      }
    }
  }

  // Draw
  int num;

  num = glGenLists(1);
  glNewList(num, GL_COMPILE);
  for (unsigned int i = 0; i < faces.size(); i++)
  {
    coordinate* vert[3];
    coordinate* normal;

    for (int j = 0; j < 3; j++)
      vert[j] = vertex[faces[i]->faces[j] - 1];

    normal = normals[faces[i]->facenum - 1];

    if (faces[i]->four)
    {
      glBegin(GL_QUADS);
      glNormal3f(normal->x, normal->y, normal->z);
      glVertex3f(vert[0]->x, vert[0]->y, vert[0]->z);
      glVertex3f(vert[1]->x, vert[1]->y, vert[1]->z);
      glVertex3f(vert[2]->x, vert[2]->y, vert[2]->z);
      glVertex3f(vertex[faces[i]->faces[3] - 1]->x, vertex[faces[i]->faces[3] - 1]->y, vertex[faces[i]->faces[3] - 1]->z);
      glEnd();
    }
    else
    {
      glBegin(GL_TRIANGLES);
      glNormal3f(normal->x, normal->y, normal->z);
      glVertex3f(vert[0]->x, vert[0]->y, vert[0]->z);
      glVertex3f(vert[1]->x, vert[1]->y, vert[1]->z);
      glVertex3f(vert[2]->x, vert[2]->y, vert[2]->z);
      glEnd();
    }
  }
  glEndList();

  for (unsigned int i = 0; i < coord.size(); i++)
    delete coord[i];
  for (unsigned int i = 0; i < vertex.size(); i++)
    delete vertex[i];
  for (unsigned int i = 0; i < normals.size(); i++)
    delete normals[i];
  for (unsigned int i = 0; i < faces.size(); i++)
    delete faces[i];

  return num;
}

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
