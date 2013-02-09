#include <iostream>
#include "Angel.h"
#include "libobj.h"

using namespace std;

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

GLMmodel* models[3];
int VBO_num_vert;
int VBO_num_norm;
int num_vert;

#define SCALE_VECTOR    0.5

GLuint vPosition;
GLuint vNormal;
int winHeight = 480;
int winWidth = 640;
bool mouseDown = false;
float xrot = 0;
float yrot = 0;
float xdiff = 0;
float ydiff = 0;
GLfloat step = 5;
// Camera Coordianate System
vec4 u = vec4(1, 0, 0, 0);
vec4 v = vec4(0, 1, 0, 0);
vec4 n = vec4(0, 0, 1, 0);
vec4 eye = vec4(0,2,10,1);

GLuint program;

// Model-view and projection matrices uniform location
GLuint  ModelViewCam, ModelViewObj, Projection, LightPosition, NormalTransformation;

// OpenGL initialization
void init()
{

  // Load the OBJ models from file

  models[0] = glmReadOBJ("../data/al.obj");
  models[1] = glmReadOBJ("../data/soccerball.obj"); 
  models[2] = glmReadOBJ("../data/tree.obj"); 

  if (!models[0]) exit(0);
  if (!models[1]) exit(0);
  if (!models[1]) exit(0);

  // Normilize vertices
  glmUnitize(models[0]);
  glmUnitize(models[1]);
  glmUnitize(models[2]);
  // Compute facet normals
  glmFacetNormals(models[0]);
  glmFacetNormals(models[1]);
  glmFacetNormals(models[2]);
  // Comput vertex normals
  glmVertexNormals(models[0], 90.0);  
  glmVertexNormals(models[1], 90.0);
  glmVertexNormals(models[2], 90.0);
  // Load the model (vertices and normals) into a vertex buffer
  glmLoadInVBO(models[0]);
  glmLoadInVBO(models[1]);
  glmLoadInVBO(models[2]);


  // Setup some sample materials
  color4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
  color4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
  color4 light_specular( 1.0, 1.0, 1.0, 1.0 );
  color4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
  color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 );
  color4 material_specular( 1.0, 0.8, 0.0, 1.0 );
  float  material_shininess = 100.0;
  color4 ambient_product = light_ambient * material_ambient;
  color4 diffuse_product = light_diffuse * material_diffuse;
  color4 specular_product = light_specular * material_specular;
  //LIGHT POSITION
  vec4 light_position_distant = vec4(0.0, 20.0, 20.0, 1.0);

  // Load shaders and use the resulting shader program
  program = InitShader( "../shaders/shader_vert.glsl", "../shaders/shader_frag.glsl" );
  glUseProgram( program );
  // set up vertex arrays
  glBindVertexArray( models[0]->vao );

  glUseProgram( program );

  glUniform4fv( glGetUniformLocation(program, "light_ambient"),1, light_ambient);
  glUniform4fv( glGetUniformLocation(program, "light_diffuse"),1, light_diffuse);
  glUniform4fv( glGetUniformLocation(program, "light_specular"),1, light_specular);	
  glUniform4fv( glGetUniformLocation(program, "lightPosition"),1, light_position_distant );
  // Retrieve transformation uniform variable locations
  ModelViewCam = glGetUniformLocation( program, "modelView" );
  ModelViewObj = glGetUniformLocation(program, "ModelViewObj");
  Projection = glGetUniformLocation( program, "projection" );
  NormalTransformation = glGetUniformLocation( program, "normalTransformation" );

  glEnable( GL_DEPTH_TEST );
  glEnable (GL_BLEND);
glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glShadeModel(GL_FLAT);
  glClearColor( 0.0, 0.0, 0.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void display( void )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Setup camera
  mat4 modelViewCamera = LookAt(eye, eye-n, v);
  glUniformMatrix4fv(ModelViewCam, 1, GL_TRUE, modelViewCamera);

 
  float scale_x, scale_y, scale_z;
  mat4 scaleTransformation;
  mat4 invScaleTranformation;
  mat4 normalMatrix;
  mat4 modelViewObject;

  // _________________________________Load the first model
  scale_x = 1;
  scale_y = 1;
  scale_z = 1;
  // Scale Transformation Matrix
  scaleTransformation = Scale(scale_x, scale_y, scale_z);
  // Inverse Scale Transformation Matrix 
  invScaleTranformation = Scale(1/scale_x, 1/scale_y, 1/scale_z);
  normalMatrix =  RotateX( xrot ) * RotateY( yrot ) * invScaleTranformation;
  modelViewObject = scaleTransformation * RotateX( xrot ) * RotateY( yrot );
  glUniformMatrix4fv( ModelViewObj, 1, GL_TRUE, modelViewObject );
  glUniformMatrix4fv(NormalTransformation , 1, GL_TRUE,  normalMatrix);
  glmDrawVBO(models[0], program);

  // _________________________________Load the second model
  scale_x = 1;
  scale_y = 1;
  scale_z = 1;
  // Scale Transformation Matrix
  scaleTransformation = Scale(scale_x, scale_y, scale_z);
  // Inverse Scale Transformation Matrix 
  invScaleTranformation = Scale(1/scale_x, 1/scale_y, 1/scale_z);
  normalMatrix = invScaleTranformation;
  modelViewObject = scaleTransformation * Translate(2, 2, 0);
  glUniformMatrix4fv( ModelViewObj, 1, GL_TRUE, modelViewObject );
  glUniformMatrix4fv(NormalTransformation , 1, GL_TRUE,  normalMatrix);
  glmDrawVBO(models[1], program);

  // _________________________________Load the third model multiple times
  scale_x = 1;
  scale_y = 1;
  scale_z =1;
  // Scale Transformation Matrix
  scaleTransformation = Scale(scale_x, scale_y, scale_z);
  // Inverse Scale Transformation Matrix 
  invScaleTranformation = Scale(1/scale_x, 1/scale_y, 1/scale_z);
  normalMatrix = invScaleTranformation;
  glUniformMatrix4fv(NormalTransformation , 1, GL_TRUE,  normalMatrix);
  modelViewObject = scaleTransformation * Translate(-5, 0, -3);
  glUniformMatrix4fv( ModelViewObj, 1, GL_TRUE, modelViewObject );
  glmDrawVBO(models[2], program);

  modelViewObject = scaleTransformation * Translate(-3, 0, -3);
  glUniformMatrix4fv( ModelViewObj, 1, GL_TRUE, modelViewObject );
  glmDrawVBO(models[2], program);

  modelViewObject = scaleTransformation * Translate(-2, 0, -5);
  glUniformMatrix4fv( ModelViewObj, 1, GL_TRUE, modelViewObject );
  glmDrawVBO(models[2], program);


  glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
  switch( key ) 
    {
    case 'X':
      break;
    case 'x':
      break;
    case 'C':
      break;
    case 'c':
      break;
    case 'Z':
      break;
    case 'z':
      break;
    case 033: // Escape Key
    case 'q': case 'Q':
      exit( EXIT_SUCCESS );
      break;
    }
  glutPostRedisplay();
}

//----------------------------------------------------------------------------

void reshape( int width, int height )
{
 glViewport( 0, 0, width, height );
  GLfloat aspect = GLfloat(width)/height;
  mat4  projection = Perspective( 45.0, aspect, 0.0001, 300.0 );
  glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
}

void specialKeys(int key, int x, int y)
{
  switch( key ) {
  case GLUT_KEY_UP: // MOVE FORWARD
    eye[0] -= SCALE_VECTOR * n[0];
    eye[1] -= SCALE_VECTOR * n[1];
    eye[2] -= SCALE_VECTOR * n[2];
    break;
  case GLUT_KEY_DOWN: // MOVE Backward
    eye[0] += SCALE_VECTOR * n[0];
    eye[1] += SCALE_VECTOR * n[1];
    eye[2] += SCALE_VECTOR * n[2];
    break;
  case GLUT_KEY_RIGHT: // MOVE right
    eye[0] += SCALE_VECTOR * u[0];
    eye[1] += SCALE_VECTOR * u[1];
    eye[2] += SCALE_VECTOR * u[2];
    break;
  case GLUT_KEY_LEFT: // MOVE left
    eye[0] -= SCALE_VECTOR * u[0];
    eye[1] -= SCALE_VECTOR * u[1];
    eye[2] -= SCALE_VECTOR * u[2];
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

void mouseMotionCB(int x, int y)
{
  if (mouseDown)
    {
      yrot = x - xdiff;
      xrot = y + ydiff;
      glutPostRedisplay();
    }
}
//----------------------------------------------------------------------------
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

// Get OpenGL version information
void getGLinfo()
{
  cout << "GL Vendor   : " << glGetString(GL_VENDOR) << endl;
  cout << "GL Renderer : " << glGetString(GL_RENDERER) << endl;
  cout << "GL Version  : " << glGetString(GL_VERSION) << endl;
}

int main( int argc, char **argv )
{
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( winWidth, winHeight );
  glutCreateWindow( "Loading OBJ Models into VBO" );
  glewInit();
  getGLinfo();
  init();
  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard );
  glutSpecialFunc (specialKeys);
  glutReshapeFunc( reshape );
  glutMouseFunc(mouseCB);
  glutMotionFunc(mouseMotionCB);
  glutMainLoop();
  return 0;
}
