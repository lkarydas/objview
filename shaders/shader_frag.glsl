#version 150 

smooth in vec3 fN;
smooth in vec3 fL;
smooth in vec3 fE;
out vec4 out_fragColor;
uniform vec4 mat_ambient;
uniform vec4 mat_diffuse;
uniform vec4 mat_specular;
uniform float mat_shininess;

uniform float mat_transparency;

uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;


void main() 
{
  vec4 AmbientProduct = light_ambient * mat_ambient;
  vec4 DiffuseProduct = light_diffuse * mat_diffuse;
  vec4 SpecularProduct = light_specular * mat_specular;
  vec3 L = normalize(fL);
  vec3 E = normalize(fE);
  vec3 N = normalize(fN);
  vec3 H = normalize( L + E );
  vec4 ambient = AmbientProduct;
  out_fragColor = ambient;
  float Kd = max(dot(L, N), 0.0);
  vec4 diffuse = Kd*DiffuseProduct;
  float Ks = pow(max(dot(N, H), 0.0), mat_shininess);
  vec4 specular = Ks*SpecularProduct;
  if( dot(L, N) < 0.0 )
    {
      specular = vec4(0.0, 0.0, 0.0, 1.0);
    }
  out_fragColor +=diffuse + specular;
  out_fragColor.a = mat_transparency;
 
} 
