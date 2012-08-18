#version 120

uniform sampler2D img;
uniform sampler2D normalmap;

varying vec2 texcoord;

varying vec3 position;
varying vec3 normal;

uniform vec3 lightPosition;

uniform vec3 mambient; // gl_FrontMaterial.diffuse
uniform vec3 mdiffuse;
uniform vec3 mspecular;
uniform float shininess;

uniform vec3 lambient; // gl_LightSource[0].diffuse
uniform vec3 ldiffuse;
uniform vec3 lspecular;

varying vec3 tangentSurface2light;
varying vec3 tangentSurface2view;

void main()
{
  vec3 texcolor = vec3(texture2D(img, texcoord));

  float distance = length(lightPosition - position);
  float attenuation = 1.0/(1.0 + 0.1*distance + 0.01*distance*distance);

  vec3 ambient = texcolor*lambient;

  vec3 surface2light = normalize(tangentSurface2light);
  vec3 norm = normalize(texture2D(normalmap, texcoord).xyz*2.0 - 1.0);
  float diffcontrib = max(0.0, dot(norm, surface2light));
  vec3 diffuse = diffcontrib*texcolor*ldiffuse;

  vec3 surface2view = normalize(tangentSurface2view);
  vec3 reflection = reflect(-surface2light, norm);
  float speccontrib = pow(max(0.0, dot(surface2view, reflection)), shininess);
  vec3 specular = speccontrib*mspecular*lspecular;

  gl_FragColor = vec4(ambient + diffuse + specular*attenuation, 1.0);
}