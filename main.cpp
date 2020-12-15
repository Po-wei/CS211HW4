#include <stdio.h>
#include <stdlib.h>
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glew\include\GL\glew.h"
#include "glut\glut.h"
#include <memory.h>
#include <math.h>
#include "mymath.h"
 
#define PI       3.14159265358979323846

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




float normals1[] = {     -1.0f,0.0f,0.0f, // triangle 1 : begin
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


 
float colors1[] = { 0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
			0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
			0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
			0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
			0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
			0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
			0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
			0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
			0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
			0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
			0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
			0.0f, 0.0f, 1.0f, 0.3f,
            0.0f, 0.0f, 1.0f, 0.3f,
            0.0f,0.0f, 1.0f, 0.3f,
};

 
// shader names
char *vertexFileName = "vertex.txt";
char *fragmentFileName = "frag.txt";
 
// program and shader Id
GLuint p,v,f;
 
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

float ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
float lightColor[] = {0.9f, 0.8f, 0.9f};
float lightPosition[] = { 4.0f, 4.0f, 4.0f};
float Shininess = 20.0;
float Strength = 10.0;
float EyeDirection[] = {0 , 0 , -5};


int frame=0,time,timebase=0;
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
 
// vector opt
// res = a cross b;
void xProduct( float *a, float *b, float *res) 
{
 
    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
}
 
// normalize a vec3
void normalize(float *a) 
{
 
    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);
    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}

// Matrix Opt. - In Opengl 3 we need to handle our own matrix.
 
// In this form : a = a * b; 
void multiplyMatrix(float *a, float *b) 
{
    float res[16];
 
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += a[k*4 + i] * b[j*4 + k];
            }
        }
    }
    memcpy(a, res, 16 * sizeof(float));
}

// sets the square matrix mat to the ID matrix,
void setIdentMatrix( float *mat, int size) 
{
	// input: size: size of the matrix (for example size=4 means 4 by 4 matrix)
	// output: mat: Identity matrix
}
 
// View Matrix
// just like glulookat
void placeCam(float posX, float posY, float posZ, float lookX, float lookY, float lookZ) 
{
 
    float dir[3], right[3], up[3];
 
    up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;
 
    dir[0] =  (lookX - posX);
    dir[1] =  (lookY - posY);
    dir[2] =  (lookZ - posZ);
    normalize(dir);
 
	// this order matters
    xProduct(dir,up,right);
    normalize(right);
 
    xProduct(right,dir,up);
    normalize(up);
 
    float aux[16];
 
    viewMatrix[0]  = right[0];
	viewMatrix[1]  = up[0];
	viewMatrix[2]  = -dir[0];
	viewMatrix[3]  = 0.0f;
    viewMatrix[4]  = right[1];
	viewMatrix[5]  = up[1];
	viewMatrix[6]  = -dir[1];
	viewMatrix[7]  = 0.0f;
    viewMatrix[8]  = right[2];
	viewMatrix[9]  = up[2];
	viewMatrix[10] = -dir[2];
	viewMatrix[11] = 0.0f;
    viewMatrix[12] = 0.0f;
    viewMatrix[13] = 0.0f;
    viewMatrix[14] =  0.0f;
    viewMatrix[15] = 1.0f;

	normalMatrix[0] = viewMatrix[0];
	normalMatrix[1] = viewMatrix[1];
	normalMatrix[2] = viewMatrix[2];
	normalMatrix[3] = viewMatrix[4];
	normalMatrix[4] = viewMatrix[5];
	normalMatrix[5] = viewMatrix[6];
	normalMatrix[6] = viewMatrix[8];
	normalMatrix[7] = viewMatrix[9];
	normalMatrix[8] = viewMatrix[10];

    setTransMatrix(aux, -posX, -posY, -posZ);
    multiplyMatrix(viewMatrix, aux);
	
}

// Generates a rotation matrix.  Angle is in radian.
glm::mat4x4 rotationMatrix(float x, float y, float z, float angle)
{
	// inputs:  x,y,z: rotation vector
	//          angle:  angle of rotation arount vector(x,y,z)
	// output:  returns rotation matrix 
	glm::mat4x4 initRot(1.0f);
	return glm::rotate(initRot, angle, glm::vec3(x, y, z));
}
 
// Projection Matrix
void buildProjMatrix(float fov, float ratio, float nearP, float farP) {
 
    float f = 1.0f / tan (fov * (PI / 360.0));
    setIdentMatrix(projMatrix,4);
    projMatrix[0] = f / ratio;
    projMatrix[1 * 4 + 1] = f;
    projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
    projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
    projMatrix[2 * 4 + 3] = -1.0f;
    projMatrix[3 * 4 + 3] = 0.0f;
}

// Transformation matrix mat with a translation
void setTransMatrix(float *mat, float x, float y, float z) {
	// inputs:  x: translation in x direction 
	//          y: translation in y direction
	//          z: translation in z direction
	// output:  mat : translation matrix 
    
}

//Transformation matrix mat with a scaling
void setScale(float *mat, float xScale, float yScale, float zScale) 
{
    // inputs:  xScale: scale in x direction 
	//          yScale: scale in y direction
	//          zScale: scale in z direction
	// output:  mat : Scale matrix
}

void changeSize(int w, int h) {
 
    float ratio;

    // place viewport to be the entire window
    glViewport(0, 0, w, h);
    ratio = (1.0f * w) / h;
    buildProjMatrix(53.13f, ratio, 1.0f, 30.0f);
}
 
void setupBuffers() {
 
    GLuint buffers[4];
 
    glGenVertexArrays(1, vert);

    // first triangle
    glBindVertexArray(vert[0]);
    // generate 2 buffers for vert and color
    glGenBuffers(4, buffers);
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
 
void setUniforms() {
 
    // must be called after glUseProgram
	// set the variables for the shader
    glUniformMatrix4fv(projMatrixLoc,  1, false, projMatrix);
    glUniformMatrix4fv(viewMatrixLoc,  1, false, viewMatrix);
	glUniformMatrix3fv(normalMatrixLoc, 1, false, normalMatrix);
	glUniform4fv(ambientColorLoc, 1, ambientColor);
	glUniform3fv(lightColorLoc, 1, lightColor);
	glUniform3fv(lightPositionLoc, 1, lightPosition);
	glUniform1f(ShininessLoc,Shininess);
	glUniform1f(StrengthLoc, Strength);
	glUniform3fv(EyeDirectionLoc,1,EyeDirection);

}
 
void renderScene(void) {

	
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",
			frame*1000.0/(time-timebase));
		timebase = time;
		frame = 0;
	}
    glutSetWindowTitle(s);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    //placeCam(10,2,10,0,2,-5);
	placeCam(viewPosition[0],viewPosition[1],viewPosition[2], 0,0,-5);
	multiplyMatrix(viewMatrix, (float*)glm::value_ptr(rotationMatrix(0.0,1.0,0.0, angle)));
	multiplyMatrix(viewMatrix, (float*)glm::value_ptr(rotationMatrix(1.0,0.0,0.0, angle2)));
    glUseProgram(p);
    setUniforms();

    glBindVertexArray(vert[0]);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices1));

	float T[16];	
	setScale(T,0.5,0.5,0.5);
	multiplyMatrix(viewMatrix, T);
	setTransMatrix(T,4,0,0);
	multiplyMatrix(viewMatrix, T);

    setUniforms();

    glBindVertexArray(vert[1]);
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
 
GLuint initShaders() {
 
    char *vertShader = NULL,*fragShader = NULL;
 
    GLuint p,v,f;
 
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    vertShader = getTxtFile(vertexFileName);
    fragShader = getTxtFile(fragmentFileName);
    const char * vv = vertShader;
    const char * ff = fragShader;
    glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);
    free(vertShader);
	free(fragShader);
    glCompileShader(v);
    glCompileShader(f);
    printShaderInfoLog(v);
    printShaderInfoLog(f);
    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);
    glBindFragDataLocation(p, 0, "outputF");
    glLinkProgram(p);
    printProgramInfoLog(p);
    vertexLoc = glGetAttribLocation(p,"position");
    colorLoc = glGetAttribLocation(p, "color"); 
	normalLoc = glGetAttribLocation(p, "normal");
    projMatrixLoc = glGetUniformLocation(p, "projMatrix");
    viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");
	normalMatrixLoc = glGetUniformLocation(p, "normalMatrix");
	ambientColorLoc = glGetUniformLocation(p,"Ambient");
	lightColorLoc = glGetUniformLocation(p,"LightColor");
	lightPositionLoc = glGetUniformLocation(p,"LightPosition");
	ShininessLoc = glGetUniformLocation(p,"Shininess");
	StrengthLoc = glGetUniformLocation(p,"Strength");
	EyeDirectionLoc = glGetUniformLocation(p,"EyeDirection");
 
    return(p);
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

    glClearColor(1.0,1.0,1.0,1.0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	init();
    p = initShaders(); 
    setupBuffers(); 
    glutMainLoop();

    return(0); 
}
