#include "objloader.h"

coordinate::coordinate(float a, float b, float c)
{
  x = a;
  y = b;
  z = c;
}

face::face(int facen, int f1, int f2, int f3, int t1, int t2, int t3, int m)
{
  facenum = facen;
  faces[0] = f1;
  faces[1] = f2;
  faces[2] = f3;
  faces[3] = -1;
  texcoord[0] = t1;
  texcoord[1] = t2;
  texcoord[2] = t3;
  texcoord[3] = -1;
  mat = m;
  four = false;
}

face::face(int facen, int f1, int f2, int f3, int f4, int t1, int t2, int t3, int t4, int m)
{
  facenum = facen;
  faces[0] = f1;
  faces[1] = f2;
  faces[2] = f3;
  faces[3] = f4;
  texcoord[0] = t1;
  texcoord[1] = t2;
  texcoord[2] = t3;
  texcoord[3] = t4;
  mat = m;
  four = true;
}

material::material(const char* na, float al, float n, float ni2, float* d, float* a, float* s, int i, int t)
{
  name = na;
  alpha = al;
  ns = n;
  ni = ni2;

  for (int j = 0; j < 3; j++)
  {
    diffuse[j] = d[j];
    ambient[j] = a[j];
    specular[j] = s[j];
  }

  illum = i;
  texture = t;
}

texcoord::texcoord(float a, float b)
{
  u = a;
  v = b;
}

objloader::objloader()
{
  ismaterial = isnormals = istexture = false;
}

int objloader::load(const char* filename)
{
  std::cout << "Reading/parsing object file: " << filename << std::endl;

  std::ifstream in(filename);
  if(!in.is_open())
  {
    std::cout << "Unable to read object file: " << filename << std::endl;

    return -1;
  }

  // Current material
  int curmat = 0;

  char buf[256];
  while (!in.eof())
  {
    in.getline(buf, 256);
    coord.push_back(new std::string(buf));
  }

  const int operationbuffer = 3;
  char operation[operationbuffer];

  for (unsigned int i = 0; i < coord.size(); i++)
  {
    for (int j = 0; j < operationbuffer; j++)
    {
      operation[j] = (*coord[i])[j];
    }

    if (operation[0] == '#') // "#" = COMMENT
      continue;
    else if (operation[0] == 'v' && operation[1] != 't')
    {
      if (operation[1] == ' ') // "v" = VERTEX
      {
	float tmpx, tmpy, tmpz;
	sscanf(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz);
	vertex.push_back(new coordinate(tmpx, tmpy, tmpz));
      }
      else if (operation[1] == 'n') // "vn" = VECTOR NORMAL
      {
	float tmpx, tmpy, tmpz;
        sscanf(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
	normals.push_back(new coordinate(tmpx, tmpy, tmpz));
      }
    }
    else if (operation[0] == 'f') // "f" = FACE
    {
      int a, b, c, d, e;
      if (count(coord[i]->begin(), coord[i]->end(), ' ') == 4) // Four spaces means four vertices; QUAD
      {
	if (coord[i]->find("//") != std::string::npos)
	{
	  sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b, &e, &b);
	  faces.push_back(new face(b, a, c, d, e, 0, 0, 0, 0, curmat));
	}
	else if (coord[i]->find("/") != std::string::npos)
	{
	  int t[4];

	  sscanf(coord[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &a, &t[0], &b, &c, &t[1], &b, &d, &t[2], &b, &e, &t[3], &b);
	  faces.push_back(new face(b, a, c, d, e, t[0], t[1], t[2], t[3], curmat));
	}
	else
	{
	  sscanf(coord[i]->c_str(), "f %d %d %d %d", &a, &b, &c, &d);
	  faces.push_back(new face(-1 , a, b, c, d, 0, 0, 0, 0, curmat));
	}
      }
      else // Assume there are three vertices; TRIANGLE
      {
	if (coord[i]->find("//") != std::string::npos) // There's no UV mapping, but there are normals
	{
	  sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b);
	  faces.push_back(new face(b, a, c, d, 0, 0, 0, curmat));
	}
	else if (coord[i]->find("/") != std::string::npos) // There is both UV mapping and normals
	{
	  int t[3];

	  sscanf(coord[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &a, &t[0], &b, &c, &t[1], &b, &d, &t[2], &b);
	  faces.push_back(new face(b, a, c, d, t[0], t[1], t[2], curmat));
	}
	else // Assume that there isn't any UV mapping or normals
	{
	  sscanf(coord[i]->c_str(), "f %d %d %d", &a, &b, &c);
	  faces.push_back(new face(-1 , a, b, c, 0, 0, 0, curmat));
	}
      }
    }
    else if (operation[0] == 'u' && operation[1] == 's' && operation[2] == 'e') // Assume "usemtl"; set material index
    {
      char materialname[200];

      sscanf(coord[i]->c_str(), "usemtl %s", materialname);
      for (unsigned int j = 0; j <  materials.size(); j++)
      {
	if (strcmp(materials[j]->name.c_str(), materialname) == 0)
	{
	  curmat = j;
	  break;
	}
      }
    }
    else if (operation[0] == 'm' && operation[1] == 't' && operation[2] == 'l') // Assume "mtllib"; parse material file and load materials
    {
      char materialfilename[200];
      sscanf(coord[i]->c_str(), "mtllib %s", materialfilename);

      std::cout << "Reading material file: " << materialfilename << std::endl;

      std::ifstream mtlin(materialfilename);
      if (!mtlin.is_open())
      {
	std::cout << "Unable to read material file:" << materialfilename << std::endl;
	clean();

	return -1;
      }

      ismaterial = true;

      std::vector<std::string> mtlline;
      char c[200];
      while(!mtlin.eof())
      {
	mtlin.getline(c, 200);
	mtlline.push_back(c);
      }

      char name[200];
      char filename[200];
      float ambient[3], diffuse[3], specular[3], alpha, ns, ni;
      int illum, texture = -1;
      bool ismat = false;

      strcpy(filename, "\0");


      const int mtloperationbuffer = 3;
      char mtloperation[mtloperationbuffer];

      for (unsigned int j = 0; j < mtlline.size(); j++)
      {
	for (int k = 0; k < mtloperationbuffer; k++)
	{
	  mtloperation[k] = mtlline[j][k];
	}

	if (mtloperation[0] == '#') // "#" = COMMENT
	  continue;
	if (mtloperation[0] == 'n' && mtloperation[1] == 'e' && mtloperation[2] == 'w') // Assume "newmtl"; create a new material object
	{
	  if(ismat) // Was a material just being parsed?
	  {
	    if(strcmp(filename, "\0") != 0) // Did the material have a texture?
	      strcpy(filename, "\0"); // Reset the texture filename
	    else
	      texture = -1; // If it didn't, the texture parameter needs to indicate such

	    materials.push_back(new material(name, alpha, ns, ni, diffuse, ambient, specular, illum, texture));
	  }
	  ismat = false;
	  sscanf(mtlline[j].c_str(), "newmtl %s", name);
	}
	else if (mtloperation[0] == 'N' && mtloperation[1] == 's')
	{
	  sscanf(mtlline[j].c_str(), "Ns %f", &ns);
	  ismat = true;
	}
	else if (mtloperation[0] == 'K' && mtloperation[1] == 'a')
	{
	  sscanf(mtlline[j].c_str(), "Ka %f %f %f", &ambient[0], &ambient[1], &ambient[2]);
	  ismat = true;
	}
	else if (mtloperation[0] == 'K' && mtloperation[1] == 'd')
	{
	  sscanf(mtlline[j].c_str(), "Kd %f %f %f", &diffuse[0], &diffuse[1], &diffuse[2]);
	  ismat = true;
	}
	else if (mtloperation[0] == 'K' && mtloperation[1] == 's')
	{
	  sscanf(mtlline[j].c_str(), "Ks %f %f %f", &specular[0], &specular[1], &specular[2]);
	  ismat = true;
	}
	else if (mtloperation[0] == 'N' && mtloperation[1] == 'i')
	{
	  sscanf(mtlline[j].c_str(), "Ni %f", &ni);
	  ismat = true;
	}
	else if (mtloperation[0] == 'd' && mtloperation[1] == ' ')
	{
	  sscanf(mtlline[j].c_str(), "d %f", &alpha);
	  ismat = true;
	}
	else if (mtloperation[0] == 'i' && mtloperation[1] == 'l') // Assume "illum"
	{
	  sscanf(mtlline[j].c_str(), "illum %d", &illum);
	  ismat = true;
	}
	else if (mtloperation[0] == 'm' && mtloperation[1] == 'a') // Assume "map_Kd"
	{
	  sscanf(mtlline[j].c_str(), "map_Kd %s", &filename[0]);
	  texture = loadTexture(filename);
	  ismat = true;
	}
      }
      if(ismat)
      {
        if(strcmp(filename, "\0") == 0)
        {
          texture = -1;
        }
        materials.push_back(new material(name, alpha, ns, ni, diffuse, ambient, specular, illum, texture));
      }
    }
    else if (operation[0] == 'v' && operation[1] == 't') // "vt" = TEXTURE COORDINATE
    {
      float u, v;

      sscanf(coord[i]->c_str(), "vt %f %f", &u, &v);
      texturecoordinate.push_back(new texcoord(u, 1-v));
      istexture = true;
    }
  }

  ismaterial = materials.size() == 0 ? false : true;

  std::cout << "Drawing object" << std::endl;

  // Draw
  int num;
  int lastmaterial = -1;

  num = glGenLists(1);
  glNewList(num, GL_COMPILE);
  for (unsigned int i = 0; i < faces.size(); i++)
  {
    if (ismaterial && lastmaterial != faces[i]->mat)
    {
      material* mat = materials[faces[i]->mat];
      float diffuse[4], ambient[4], specular[4];

      for (int j = 0; j < 3; j++)
      {
	diffuse[j] = mat->diffuse[j];
	ambient[j] = mat->ambient[j];
	specular[j] = mat->specular[j];
      }

      diffuse[3] = ambient[3] = specular[3] = 1.0;

      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
      glMaterialf(GL_FRONT, GL_SHININESS, mat->ns);

      lastmaterial = faces[i]->mat;

      if (mat->texture == -1) // If there is no texture
	glDisable(GL_TEXTURE_2D);
      else
      {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mat->texture);
      }
    }


    coordinate* vert[4];
    coordinate* normal;

    for (int j = 0; j < 4; j++)
      if (faces[i]->faces[j] != -1)
	vert[j] = vertex[faces[i]->faces[j] - 1];

    normal = normals[faces[i]->facenum - 1];

    if (faces[i]->four)
    {
      glBegin(GL_QUADS);
      glNormal3f(normal->x, normal->y, normal->z);
      for (int j = 0; j < 4; j++)
      {
        if (istexture && materials[faces[i]->mat]->texture != -1)
	{
	  texcoord* tc = texturecoordinate[faces[i]->texcoord[j] - 1];
	  glTexCoord2f(tc->u, tc->v);
	}

	glVertex3f(vert[j]->x, vert[j]->y, vert[j]->z);
      }
 //glVertex3f(vertex[faces[i]->faces[3] - 1]->x, vertex[faces[i]->faces[3] - 1]->y, vertex[faces[i]->faces[3] - 1]->z);
      glEnd();
    }
    else
    {
      glBegin(GL_TRIANGLES);
      glNormal3f(normal->x, normal->y, normal->z);
      for (int j = 0; j < 3; j++)
      {
	if (istexture && materials[faces[i]->mat]->texture != -1)
	{
	  texcoord* tc = texturecoordinate[faces[i]->texcoord[j] - 1];
	  glTexCoord2f(tc->u, tc->v);
	}

	glVertex3f(vert[j]->x, vert[j]->y, vert[j]->z);
      }
      glEnd();
    }
  }
  glEndList();

  std::cout << "Draw complete" << std::endl;

  clean();
  lists.push_back(num);
  return num;
}

unsigned int objloader::loadTexture(const char* filename)
{
  SDL_Surface* image = SDL_LoadBMP(filename);

  unsigned int id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, image->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  SDL_FreeSurface(image);

  textures.push_back(id);

  return id;
}

void objloader::clean()
{
  for (unsigned int i = 0; i < coord.size(); i++)
    delete coord[i];
  for (unsigned int i = 0; i < vertex.size(); i++)
    delete vertex[i];
  for (unsigned int i = 0; i < normals.size(); i++)
    delete normals[i];
  for (unsigned int i = 0; i < faces.size(); i++)
    delete faces[i];
  for (unsigned int i = 0; i < materials.size(); i++)
    delete materials[i];
  for (unsigned int i = 0; i < texturecoordinate.size(); i++)
    delete texturecoordinate[i];

  coord.clear();
  faces.clear();
  normals.clear();
  vertex.clear();
  materials.clear();
  texturecoordinate.clear();
}

objloader::~objloader()
{
  for (std::vector<unsigned int>::const_iterator it = textures.begin(); it != textures.end(); it++)
  {
    glDeleteTextures(1, &(*it));
  }
  for (std::vector<unsigned int>::const_iterator it = lists.begin(); it != lists.end(); it++)
  {
    glDeleteLists(*it, 1);
  }
}
