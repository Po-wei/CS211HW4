#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <memory.h>
#include <string>
#include <map>
#include <math.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include "mymath.h"
 
#define PI  3.14159265358979323846

void setTransMatrix(float *mat, float x, float y, float z);
void multiplyMatrix(float *a, float *b);
void xProduct( float *a, float *b, float *res);
void normalize(float *a);
void setIdentMatrix( float *mat, int size);
void placeCam(float posX, float posY, float posZ, float lookX, float lookY, float lookZ);
glm::mat4x4 rotationMatrix(float x, float y, float z, float angle);
void init();
GLuint loadBMP_custom(const char * imagepath);

struct vec3
{
	float x;
	float y;
	float z;
};

// vertices for triangle
float vertices1[] = {       -1.0f,-1.0f,-1.0f, // triangle 1 : begin
							-1.0f,-1.0f, 1.0f,
							-1.0f, 1.0f, 1.0f, // triangle 1 : end

							1.0f, 1.0f,-1.0f, // triangle 2 : begin
							-1.0f,-1.0f,-1.0f,
							-1.0f, 1.0f,-1.0f, // triangle 2 : end

							1.0f,-1.0f, 1.0f,
							-1.0f,-1.0f,-1.0f,
							1.0f,-1.0f,-1.0f,

							1.0f, 1.0f,-1.0f,
							1.0f,-1.0f,-1.0f,
							-1.0f,-1.0f,-1.0f,

							-1.0f,-1.0f,-1.0f,
							-1.0f, 1.0f, 1.0f,
							-1.0f, 1.0f,-1.0f,

							1.0f,-1.0f, 1.0f,
							-1.0f,-1.0f, 1.0f,
							-1.0f,-1.0f,-1.0f,

							-1.0f, 1.0f, 1.0f,
							-1.0f,-1.0f, 1.0f,
							1.0f,-1.0f, 1.0f,

							1.0f, 1.0f, 1.0f,
							1.0f,-1.0f,-1.0f,
							1.0f, 1.0f,-1.0f,

							1.0f,-1.0f,-1.0f,
							1.0f, 1.0f, 1.0f,
							1.0f,-1.0f, 1.0f,

							1.0f, 1.0f, 1.0f,
							1.0f, 1.0f,-1.0f,
							-1.0f, 1.0f,-1.0f,

							1.0f, 1.0f, 1.0f,
							-1.0f, 1.0f,-1.0f,
							-1.0f, 1.0f, 1.0f,

							1.0f, 1.0f, 1.0f,
							-1.0f, 1.0f, 1.0f,
							1.0f,-1.0f, 1.0f
						};




float normals1[] = {    -1.0f,0.0f,0.0f, // triangle 1 : begin
						-1.0f,0.0f,0.0f,
						-1.0f,0.0f,0.0f, // triangle 1 : end

						0.0f,0.0f,-1.0f, // triangle 2 : begin
						0.0f,0.0f,-1.0f,
						0.0f,0.0f,-1.0f, // triangle 2 : end

						0.0f,-1.0f,0.0f,
						0.0f,-1.0f,0.0f,
						0.0f,-1.0f,0.0f,

						0.0f,0.0f,-1.0f,
						0.0f,0.0f,-1.0f,
						0.0f,0.0f,-1.0f,

						-1.0f,0.0f,0.0f,
						-1.0f,0.0f,0.0f,
						-1.0f,0.0f,0.0f,

						0.0f,-1.0f,0.0f,
						0.0f,-1.0f,0.0f,
						0.0f,-1.0f,0.0f,

						0.0f,0.0f, 1.0f,
						0.0f,0.0f, 1.0f,
						0.0f,0.0f, 1.0f,

						1.0f,0.0f,0.0f,
						1.0f,0.0f,0.0f,
						1.0f,0.0f,0.0f,

						1.0f,0.0f,0.0f,
						1.0f,0.0f,0.0f,
						1.0f,0.0f,0.0f,

						0.0f, 1.0f,0.0f,
						0.0f, 1.0f,0.0f,
						0.0f, 1.0f,0.0f,

						0.0f, 1.0f,0.0f,
						0.0f, 1.0f,0.0f,
						0.0f, 1.0f,0.0f,

						0.0f,0.0f, 1.0f,
						0.0f,0.0f, 1.0f,
						0.0f,0.0f, 1.0f
						};


 
float colors1[] = {
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
	0.0f, 0.0f, 1.0f, 0.3f,
};
 
// program and shader Id
std::map<char, GLuint> programs;
 
// vert attrib locations
GLuint vertexLoc, colorLoc, normalLoc;
 
// uniform var locations
GLuint projMatrixLoc, viewMatrixLoc, normalMatrixLoc;

GLuint ambientColorLoc,lightColorLoc, lightPositionLoc, ShininessLoc, StrengthLoc, EyeDirectionLoc;

GLuint textureLoc,samplerLoc;
 
// vert array obj Id
GLuint vert[3];
 
// storage for matrices
float projMatrix[16];
float viewMatrix[16];
float normalMatrix[9];

glm::vec3 ambientColor(0.5f, 0.5f, 0.5f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 lightPosition(0.0f, 0.5f, 2.1f);
glm::vec3 camaraPos(3.0f, 5.0f, 5.0f);
glm::vec3 eyePosition(0.0f, 0.0f, 0.0f);
float shininess = 100.0;
float strength = 20.0;

int frame=0, elapseTime, timebase=0;
char s[50];

int viewPosition[3];
float angle = 0.0f;
float angle2 = 0.0f;
int startX;
int startY;

int selectX;
int selectY;
int window_width;
int window_height;
GLuint texID;

void changeSize(int w, int h) {
    // place viewport to be the entire window
    glViewport(0, 0, w, h);
}
 
void setupBuffers() {
    GLuint buffers[3];
 
    glGenBuffers(3, buffers);
    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
 
    // bind buffer for colors and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors1), colors1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);

	// bind buffer for normals and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals1), normals1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normalLoc);
    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);
}
 
static void setUniformMat4(unsigned int program, const std::string &name, const glm::mat4 &mat)
{
	glUseProgram(program);
	GLint loc=glGetUniformLocation(program, name.c_str());
	if(loc==-1) return;
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

static void setUniformMat3(unsigned int program, const std::string &name, const glm::mat3 &mat)
{
	glUseProgram(program);
	GLint loc=glGetUniformLocation(program, name.c_str());
	if(loc==-1) return;
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}
 
static void setUniformVec3(unsigned int program, const std::string &name, const glm::vec3 &vec)
{
	glUseProgram(program);
	GLint loc=glGetUniformLocation(program, name.c_str());
	if(loc==-1) return;
	glUniform3fv(loc, 1, glm::value_ptr(vec));
}

static void setUniformFloat(unsigned int program, const std::string &name, const float &scalar)
{
	glUseProgram(program);
	GLint loc=glGetUniformLocation(program, name.c_str());
	if(loc==-1) return;
	glUniform1f(loc, scalar);
}

void renderScene(void) {
	frame++;
	elapseTime=glutGet(GLUT_ELAPSED_TIME);
	if (elapseTime - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",
			frame*1000.0/(elapseTime-timebase));
		timebase = elapseTime;
		frame = 0;
	}
    glutSetWindowTitle(s);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programs['a']);

	// Vertex
	glm::mat4 projectionMat = glm::perspective(glm::radians(53.0f), 640.0f/480, 1.0f, 30.f);
	glm::mat4 viewMat = glm::lookAt(camaraPos, eyePosition, glm::vec3(0, 1, 0));
	setUniformMat4(programs['a'], "projMatrix", projectionMat);
	setUniformMat4(programs['a'], "viewMatrix", viewMat);
	const float* viewPtr = glm::value_ptr(viewMat);
	glm::mat3 normMat = glm::mat3();
	normMat[0] = glm::vec3(viewMat[0]);
	normMat[1] = glm::vec3(viewMat[1]);
	normMat[2] = glm::vec3(viewMat[2]);
	setUniformMat3(programs['a'], "normalMatrix", normMat);

	// Fragment
	setUniformVec3(programs['a'], "Ambient", ambientColor);
	setUniformVec3(programs['a'], "LightColor", lightColor);
	setUniformVec3(programs['a'], "LightPosition", lightPosition);
	setUniformVec3(programs['a'], "CamaraPos", camaraPos);
	setUniformFloat(programs['a'], "Shininess", shininess);
	setUniformFloat(programs['a'], "Strength", strength);

    //glBindVertexArray(vert[0]);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices1));

	/*
	float T[16];	
	setScale(T,0.5,0.5,0.5);
	multiplyMatrix(viewMatrix, T);
	setTransMatrix(T,4,0,0);
	multiplyMatrix(viewMatrix, T);

    setUniforms();

    glBindVertexArray(vert[1]);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices1));
	*/

   	glutSwapBuffers();
}
 

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}
 
void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
 
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
 
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}
 
GLuint initShaders(const char* vertexFile, const char* fragmentFile) {
    char *vertShader = NULL,*fragShader = NULL;
    GLuint v = glCreateShader(GL_VERTEX_SHADER);
    GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
    vertShader = getTxtFile(vertexFile);
    fragShader = getTxtFile(fragmentFile);
    const char * vv = vertShader;
    const char * ff = fragShader;
    glShaderSource(v, 1, &vv, NULL);
    glShaderSource(f, 1, &ff, NULL);
    free(vertShader);
	free(fragShader);
    glCompileShader(v);
    glCompileShader(f);
    printShaderInfoLog(v);
    printShaderInfoLog(f);
    GLuint p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);
    glBindFragDataLocation(p, 0, "outputF");
    glLinkProgram(p);
    printProgramInfoLog(p);
    vertexLoc = glGetAttribLocation(p,"position");
    colorLoc = glGetAttribLocation(p, "color"); 
	normalLoc = glGetAttribLocation(p, "normal");
 
    return p;
}

float deltaAngle = 0.0f;
int xOrigin = -1;



//This event will trigger when you have a mouse button pressed down.
void mouseMove(int x, int y) 
{
	// x and y is the mouse position.
	//printf("%d ,  %d \n",x,y);
	int motionMode = 1;
	switch(motionMode){
  case 0:
    /* No mouse button is pressed... do nothing */
    /* return; */
    break;

  case 1:
    /* rotating the view*/
	angle = angle + (x - startX)/2;
    angle2 = angle2 + (y - startY)/2;
    startX = x;
    startY = y;
    break;

  case 2:

    break;

  case 3:

    break;
  }
	
}

//This event occur when you press a mouse button.
void mouseButton(int button, int state, int x, int y) 
{
	startX = x;
	startY = y;
	selectX = x;
	selectY = y;
	//printf("%d , %d",selectX,selectY);
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) 
	{
		// when the button is released
		if (state == GLUT_UP) 
		{
			  window_width = glutGet(GLUT_WINDOW_WIDTH);
			  window_height = glutGet(GLUT_WINDOW_HEIGHT);
 
			  GLbyte color[4];
			  GLfloat depth;
			  GLuint index;
 
			  glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
			  glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			  glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
 
			  printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
					 x, y, color[0], color[1], color[2], color[3], depth, index);
					}
		else  // state = GLUT_DOWN	
		{
		}
	}
}

void keyboardAction(unsigned char key, int x, int y)
{
	printf("Keyboard press %c \n", key);
}


void init(){
	viewPosition[0] = 0;
	viewPosition[1] = 0;
	viewPosition[2] = 11;
	
}
 
int main(int argc, char **argv) 
{
	// sets up glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(640,480);
    glutCreateWindow("ICS Graphics");
    glutSetWindowTitle(s);
	// call back functions
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboardAction);

	// check if a particular extension is available on your platform
    glewInit();
    if (glewIsSupported("GL_VERSION_3_3"))
        printf("OpenGL 3.3 is supported\n");
    else 
	{
        printf("OpenGL 3.3 not supported\n");
        exit(1);
    }
    glEnable(GL_DEPTH_TEST);

	// black background
    glClearColor(0.2, 0.2, 0.2, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	init();
	programs['a'] = initShaders("vertexA.txt", "fragA.txt");
    setupBuffers(); 
    glutMainLoop();

    return(0); 
}
