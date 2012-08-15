#version 120

uniform vec3 color;
varying vec3 color2;

void main()
{
  gl_FragColor = vec4(color2, 1.0);
}