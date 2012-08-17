#version 120

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

void main()
{
  float distance = length(lightPosition - position);
  float attenuation = 1.0/(1.0 + 0.1*distance + 0.01*distance*distance);

  vec3 ambient = mambient*lambient;

  vec3 surface2light = normalize(lightPosition - position);
  vec3 norm = normalize(normal);
  float diffcontrib = max(0.0, dot(norm, surface2light));
  vec3 diffuse = diffcontrib*mdiffuse*ldiffuse;

  vec3 surface2view = normalize(vec3(0.0, 0.0, 0.0) - position);
  vec3 reflection = reflect(-surface2light, norm);
  float speccontrib = pow(max(0.0, dot(surface2view, reflection)), shininess);
  vec3 specular = speccontrib*mspecular*lspecular;

  gl_FragColor = vec4(ambient + diffuse + specular*attenuation, 1.0);
}