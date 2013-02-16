// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>



// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include "shader.hpp"

#include "libobj.h"

GLMmodel* objModel;

GLuint programID;
GLuint modelID;
GLuint viewID;
GLuint projectionID;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

GLuint vertexbuffer;
GLuint normalbuffer;

// Rotation interface
float xrot = 0;
float yrot = 0;
float xdiff = 0;
float ydiff = 0;
bool mouseDown = false;

void mouseMotionCB(int x, int y)
{
  if (mouseDown)
    {
      yrot = x - xdiff;
      xrot = y + ydiff;
      glutPostRedisplay();
    }
}

void mouseCB(int button, int state, int x, int y)
{
  switch(button){
  case GLUT_LEFT_BUTTON:
    switch(state)
      {
      case GLUT_DOWN:
	mouseDown = true;
	xdiff = x - yrot;
	ydiff = -y + xrot;
	break;
      case GLUT_UP:
	mouseDown = false;
	break;
      default:
	break;
      }
    break;    
  case GLUT_MIDDLE_BUTTON:
    break;
  default:
    break;
  }
}

void display(void)

{

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 xRotMat = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1, 0, 0) );
  glm::mat4 yRotMat = glm::rotate(glm::mat4(1.0f), yrot, glm::vec3(0, 1, 0) );
  model = xRotMat * yRotMat;
  glm::mat4 modelViewMatrix = view * model;
  glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix)); // Normal Matrix

  // Use our shader
  glUseProgram(programID);
  // Send the model, view and projection matrices to the shader 
  glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
  glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix3fv( glGetUniformLocation(programID, "normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
  glmDrawVBO(objModel, programID);

  // Swap buffers
  glutSwapBuffers();

}

void init()
{

  objModel = glmReadOBJ("../data/al.obj");
  if (!objModel) exit(0);


  // Normilize vertices
  glmUnitize(objModel);
  // Compute facet normals
  glmFacetNormals(objModel);
  // Comput vertex normals
  glmVertexNormals(objModel, 90.0);
  // Load the model (vertices and normals) into a vertex buffer
  glmLoadInVBO(objModel);

  // Black background
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS); 

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders( "../shaders/vertShader.glsl", "../shaders/fragShader.glsl" );

  // Get a handle for our model, view and projection uniforms
  modelID = glGetUniformLocation(programID, "model");
  viewID = glGetUniformLocation(programID, "view");
  projectionID = glGetUniformLocation(programID, "projection");

  glm::vec4 light_ambient = glm::vec4( 0.1, 0.1, 0.1, 0.5 );
  glm::vec4 light_diffuse = glm::vec4 ( 0.8, 1.0, 1.0, 1.0 );
  glm::vec4 light_specular =glm::vec4( 0.8, 1.0, 1.0, 1.0 );

  glUseProgram(programID);
  glUniform4fv( glGetUniformLocation(programID, "light_ambient"),1,&light_ambient[0]);
  glUniform4fv( glGetUniformLocation(programID, "light_diffuse"),1, &light_diffuse[0]);
  glUniform4fv( glGetUniformLocation(programID, "light_specular"),1, &light_specular[0]);      

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  // Camera matrix
  view = glm::lookAt( glm::vec3(0,1,3), // Camera is at (4,3,-3), in World Space
		      glm::vec3(0,0,0), // and looks at the origin
		      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		      );
  // Model matrix : an identity matrix (model will be at the origin)
  model      = glm::mat4(1.0f);

  // Initialize a light
  glm::vec4 lightPosition = glm::vec4(-20, -10, 0, 0);
  glUniform4fv( glGetUniformLocation(programID, "lightPos"),1, &lightPosition[0]); 

}


// Get OpenGL version information
void getGLinfo()
{
  std::cout << "GL Vendor   : " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "GL Renderer : " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "GL Version  : " << glGetString(GL_VERSION) << std::endl;
}
void reshape( int width, int height )
{
  glViewport(0, 0, width, height);
  GLfloat aspectRatio = GLfloat(width)/height;
  projection = glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);
  glutPostRedisplay();
}
int main(  int argc, char **argv  )
{


  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( 640, 480 );
  glutCreateWindow( "Bottom Up" );
  glewInit();
  getGLinfo();
  init();
  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutMouseFunc(mouseCB);
  glutMotionFunc(mouseMotionCB);
  glutMainLoop();
  return 0;




    
}
