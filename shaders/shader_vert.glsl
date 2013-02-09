#version 150

in vec4 vertex;
in vec3 normal;
smooth out vec3 fN;
smooth out vec3 fE;
smooth out vec3 fL;
uniform mat4 modelView;
uniform mat4 ModelViewObj;
uniform vec4 lightPosition;
uniform mat4 projection;
uniform mat4 normalTransformation;

void main()
{
  vec3 pos = (modelView*ModelViewObj*vertex).xyz;  
  fN = normalize((normalTransformation*vec4(normal,0.0)).xyz);
  fL = -(modelView*lightPosition).xyz;
  if (lightPosition.w != 0)
    {
      fL = (modelView*lightPosition).xyz - pos;
    }
  fE =-pos;
  gl_Position = projection*modelView*ModelViewObj*vertex;
}
