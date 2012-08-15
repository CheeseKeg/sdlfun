#version 120

varying vec3 color2;
attribute vec3 atr;

void main()
{
  gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
  color2 = atr; 
}