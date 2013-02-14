#version 330 core



in vec3 fN;
in vec3 fL;
in vec3 fE;

// Ouput data
out vec4 color;

uniform vec4 mat_ambient;
uniform vec4 mat_diffuse;
uniform vec4 mat_specular;
uniform float mat_shininess;

uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;


void main()
{

  
  vec4 AmbientProduct = light_ambient * mat_ambient;
  vec4 DiffuseProduct = light_diffuse * mat_diffuse;
  vec4 SpecularProduct = light_specular * mat_specular;

  //vec4 AmbientProduct = vec4(0.2, 0.2, 0.2, 1.0);
  //vec4 DiffuseProduct = vec4(0.5, 0.5, 0.5, 1.0);
  //vec4 SpecularProduct = vec4(0.5, 0.5, 0.5, 1.0);
  vec3 L = normalize(fL);
  vec3 E = normalize(fE);
  vec3 N = normalize(fN);
  vec3 H = normalize( L + E );
  vec4 ambient = AmbientProduct;
  color = ambient;
  float Kd = max(dot(L, N), 0.0);
  vec4 diffuse = Kd*DiffuseProduct;
  float Ks = pow(max(dot(N, H), 0.0), 6.0);
  vec4 specular = Ks*SpecularProduct;
  if( dot(L, N) < 0.0 )
    {
      specular = vec4(0.0, 0.0, 0.0, 1.0);
    }
  color +=diffuse + specular;
  
  color.a = 1.0f;


}
