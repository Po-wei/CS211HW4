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
#include "glm/gtx/euler_angles.hpp"


#include "mymath.h"
 
#define PI  3.14159265358979323846

GLuint loadBMP_custom(const char * imagepath);
// vertices for triangle
float vertices1[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
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

float normals1[] = {
	-1.0f,0.0f,0.0f, // triangle 1 : begin
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

char currentKey = 'a';
 
// storage for matrices
float projMatrix[16];
float viewMatrix[16];
float normalMatrix[9];

glm::vec3 ambientColor(0.5f, 0.5f, 0.5f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 lightPosition(0.0f, 4.0f, 6.0f);
glm::vec3 camaraPos(0.0f, 0.0f, 8.0f);
glm::vec3 eyePosition(0.0f, 0.0f, 0.0f);
float shininess = 2000.0;
float strength = 0.1;

int viewPosition[3];
float angleX = 0.0f, angleY = 0.0f;
int startX;
int startY;
bool released = true;

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
	static int timebase=glutGet(GLUT_ELAPSED_TIME);
	static unsigned int ticks = 0;
	if (glutGet(GLUT_ELAPSED_TIME) != timebase && released) {
		ticks += 1;
	}

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programs[currentKey]);

	// Vertex
	glm::mat4 projectionMat = glm::perspective(glm::radians(53.0f), 640.0f/480, 1.0f, 30.f);
	glm::mat4 camaraMat = glm::lookAt(camaraPos, eyePosition, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 modelMat = glm::rotate_slow(glm::mat4(1.0f), (float) ticks / 100, glm::vec3(0.0f, 1.0f, 0.0f));
	setUniformMat4(programs[currentKey], "projMatrix", projectionMat);
	// Camara rotate
	glm::vec4 camaraUp = camaraMat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	glm::vec4 camaraLeft = camaraMat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	glm::mat4 camaraRotate = glm::rotate(glm::mat4(1.0f), angleX / 10, glm::vec3(camaraUp));
	camaraRotate = glm::rotate(camaraRotate, angleY / 10, glm::vec3(camaraLeft));
	glm::vec4 realCamaraPos = camaraRotate * glm::vec4(camaraPos, 1.0f);
	glm::mat4 viewMat = glm::lookAt(glm::vec3(realCamaraPos), eyePosition, glm::vec3(0.0f, 1.0f, 0.0f));
	setUniformMat4(programs[currentKey], "viewMatrix", viewMat);
	setUniformMat4(programs[currentKey], "modelMatrix", modelMat);
	const float* viewPtr = glm::value_ptr(viewMat);
	glm::mat3 normMat = glm::mat3();
	normMat[0] = glm::vec3(viewMat[0]);
	normMat[1] = glm::vec3(viewMat[1]);
	normMat[2] = glm::vec3(viewMat[2]);
	setUniformMat3(programs[currentKey], "normalMatrix", normMat);

	// Fragment
	setUniformVec3(programs[currentKey], "Ambient", ambientColor);
	setUniformVec3(programs[currentKey], "LightColor", lightColor);
	setUniformVec3(programs[currentKey], "LightPosition", lightPosition);
	setUniformVec3(programs[currentKey], "CamaraPos", camaraPos);
	setUniformFloat(programs[currentKey], "Shininess", shininess);
	setUniformFloat(programs[currentKey], "Strength", strength);

	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices1));

	modelMat = glm::scale(modelMat, glm::vec3(0.5f, 0.5f, 0.5f));
	modelMat = glm::translate(modelMat, glm::vec3(5.0f, 0.0f, 5.0f));
	setUniformMat4(programs[currentKey], "modelMatrix", modelMat);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices1));

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

//This event will trigger when you have a mouse button pressed down.
void mouseMove(int x, int y) 
{
	// x and y is the mouse position.
	int motionMode = 1;
	switch(motionMode){
		case 0:
			/* No mouse button is pressed... do nothing */
			/* return; */
			break;

		case 1:
			/* rotating the view*/
			angleX = angleX + (x - startX)/2;
			angleY = angleY + (y - startY)/2;
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
#define WHEEL_UP 3
#define WHEEL_DOWN 4
#define WHEEL_STEP 0.2f
void mouseButton(int button, int state, int x, int y) 
{
	startX = x;
	startY = y;
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON){
		// when the button is released
		if (state == GLUT_UP) {
			released = true;
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
		}else{  // state = GLUT_DOWN	
			released = false;
		}
	}else if(button == WHEEL_UP){
		camaraPos.z -= WHEEL_STEP;
	}else if(button == WHEEL_DOWN){
		camaraPos.z += WHEEL_STEP;
	}
}

void keyboardAction(unsigned char key, int x, int y)
{
	switch (key){
		case 'a':
		case 'p':
		case 'g':
		case 'A':
		case 'P':
		case 'G':
			currentKey = key;
		break;
	}
	printf("Keyboard press %c \n", key);
}
 
int main(int argc, char **argv) 
{
	// sets up glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("ICS Graphics");
    glutSetWindowTitle("Assignment 4");
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
	programs['a'] = initShaders("vertexA.txt", "fragA.txt");
	programs['p'] = initShaders("vertexP.txt", "fragP.txt");
	programs['g'] = initShaders("vertexG.txt", "fragG.txt");
	programs['A'] = programs['a'];
	programs['P'] = programs['p'];
	programs['G'] = programs['g'];
    setupBuffers(); 
    glutMainLoop();

    return(0); 
}
