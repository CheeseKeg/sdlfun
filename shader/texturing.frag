#version 120

uniform sampler2D image;
uniform sampler2D image2;
varying vec2 texcoord;

void main()
{
  vec4 texcolor = texture2D(image, texcoord);
  vec4 texcolor2 = texture2D(image2, texcoord);
  gl_FragColor = texcolor*0.5 + texcolor2*0.5;
}