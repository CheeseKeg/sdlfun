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

unsigned int loadTexture(const char* filename, bool clamp)
{
  SDL_Surface* image = IMG_Load(filename);

  if (image == NULL)
  {
    std::cout << "Unable to load image file: " << filename << std::endl;
    return 0;
  }

  SDL_PixelFormat form = {NULL, 32, 4, 0, 0, 0, 0, 8, 8, 8, 8, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff, 0, 255};
  SDL_Surface* convert = SDL_ConvertSurface(image, &form, SDL_SWSURFACE);
  if (convert == NULL)
  {
    std::cout << "Unable to convert image data loaded from: " << filename << std::endl;
    return -1;
  }

  unsigned int id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (clamp)
  {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convert->w, convert->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, convert->pixels);

  SDL_FreeSurface(image);
  SDL_FreeSurface(convert);

  return id;
}

// Load 32 bit height bitmaps
void loadHeightmap(const char* filename, std::vector< std::vector<float> >& heights)
{
  SDL_Surface* image = SDL_LoadBMP(filename);

  if (!image)
  {
    std::cout << "Unable to load image: " << filename << std::endl;
    return;
  }

  std::vector<float> temp;
  for (int i = 0; i < image->h; i++)
  {
    temp.clear();
    for (int j = 0; j < image->w; j++)
    {
      Uint32 pixel = ((Uint32*)image->pixels)[i*image->pitch/4 + j];
      Uint8 r, g, b; // Unsigned char

      SDL_GetRGB(pixel, image->format, &r, &g, &b);

      temp.push_back((float)r/255.0);
    }
    heights.push_back(temp);
  }

  return;
}

void renderHeightmap(float size, float h, std::vector<std::vector<float> >& heights)
{
  for (unsigned int i = 0; i < heights.size() - 1; i++)
  {
    for (unsigned int j = 0; j < heights[0].size() - 1; j++)
    {
      glBegin(GL_TRIANGLE_STRIP);
      for (int k = 0; k < 2; k++)
      {
	for (int l = 0; l < 2; l++)
	{
	  //glColor3f(heights[i + l][j + k], 1 - heights[i + l][j + k], 0.0); // Green to red
	  float col[] = {heights[i + l][j + k], 1 - heights[i + l][j + k], 0.0, 1.0};
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, col);

	  glVertex3f((i + l)*size, heights[i + l][j + k]*h, (j + k)*size);
	}
      }
      glEnd();
    }
  }
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

bool rayplane(float nx, float ny, float nz, float xs, float ys, float zs, float xd, float yd, float zd, coordinate p1, coordinate p2, coordinate p3, coordinate p4, float* dist, coordinate* point)
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
  if(abs(trianglearea(p1, p3, p4) - trianglearea(p1, p4, cp) - trianglearea(p1, p3, cp) - trianglearea(p3, p4, cp)) < 0.000001 || abs(trianglearea(p1, p2, p3) - trianglearea(p1, p2, cp) - trianglearea(p2, p3, cp) - trianglearea(p1, p3, cp)) < 0.000001)
  {
    if (dist != NULL)
    {
      (*dist) = t;

      if (point != NULL)
      {
	point->x = x;
	point->y = y;
	point->z = z;
      }
    }

    return true;
  }

  return false;
}

bool sphereplane(coordinate& sp, coordinate vn, coordinate p1, coordinate p2, coordinate p3, coordinate p4, float r)
{
  float dist1, dist2;
  dist1 = dist2 = 0;

  if (rayplane(-vn.x, -vn.y, -vn.z, sp.x, sp.y, sp.z, vn.x, vn.y, vn.z, p1, p2, p3, p4, &dist1, NULL) || rayplane(vn.x, vn.y, vn.z, sp.x, sp.y, sp.z, -vn.x, -vn.y, -vn.z, p1, p2, p3, p4, &dist2, NULL))
  {
    if (dist1 > r || dist2 > r)
      return false;

    if (dist1 > 0)
    {
      const float offdist = r - dist1;
      sp.x = sp.x - vn.x*(offdist);
      sp.y = sp.y - vn.y*(offdist);
      sp.z = sp.z - vn.z*(offdist);
    }
    else
    {
      const float offdist = r - dist2;
      sp.x = sp.x + vn.x*(offdist);
      sp.y = sp.y + vn.y*(offdist);
      sp.z = sp.z + vn.z*(offdist);
    }

    return 1;
  }

  return 0;
}

float trianglearea(coordinate p1,coordinate p2,coordinate p3)
{
        // Heron's formula
        float a=sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y)+(p2.z-p1.z)*(p2.z-p1.z));
        float b=sqrt((p3.x-p2.x)*(p3.x-p2.x)+(p3.y-p2.y)*(p3.y-p2.y)+(p3.z-p2.z)*(p3.z-p2.z));
        float c=sqrt((p1.x-p3.x)*(p1.x-p3.x)+(p1.y-p3.y)*(p1.y-p3.y)+(p1.z-p3.z)*(p1.z-p3.z));
        float s=(a+b+c)/2;
        return (sqrt(s*((s-a)*(s-b)*(s-c))));
}
