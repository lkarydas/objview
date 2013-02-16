#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition; // Vertex position in model space
layout(location = 1) in vec3 vertexNormal;



// Values that stay constant for the whole mesh.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform vec4 lightPos;

out vec3 fN;
out vec3 fE;
out vec3 fL;

void main()
{	
  vec3 pos = (view * model * vec4(vertexPosition,1)).xyz;
  fN = normalize(normalMatrix * vertexNormal);
  fL = -(view * lightPos).xyz;
  fE = -pos;
  // Output position of the vertex, in clip space : MVP * position
  
  gl_Position =  projection * view * model * vec4(vertexPosition,1);
  
}

