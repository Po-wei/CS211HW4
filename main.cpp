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

#include "tiny_obj_loader.h"
#include "mymath.h"
 
#define PI  3.14159265358979323846
 
// program and shader Id
std::map<char, GLuint> programs;
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

struct object_struct{
	unsigned int vao;
	unsigned int vbo[4];
	unsigned int texture;
	unsigned int indiceCount;
} ;

static unsigned char *load_bmp(const char *bmp, unsigned int *width, unsigned int *height, unsigned short int *bits)
{
	unsigned char *result=nullptr;
	FILE *fp = fopen(bmp, "rb");
	if(!fp)
		return nullptr;
	char type[2];
	unsigned int size, offset;
	// check for magic signature	
	fread(type, sizeof(type), 1, fp);
	if(type[0]==0x42 || type[1]==0x4d){
		fread(&size, sizeof(size), 1, fp);
		// ignore 2 two-byte reversed fields
		fseek(fp, 4, SEEK_CUR);
		fread(&offset, sizeof(offset), 1, fp);
		// ignore size of bmpinfoheader field
		fseek(fp, 4, SEEK_CUR);
		fread(width, sizeof(*width), 1, fp);
		fread(height, sizeof(*height), 1, fp);
		// ignore planes field
		fseek(fp, 2, SEEK_CUR);
		fread(bits, sizeof(*bits), 1, fp);
		unsigned char *pos = result = new unsigned char[size-offset];
		fseek(fp, offset, SEEK_SET);
		while(size-ftell(fp)>0)
			pos+=fread(pos, 1, size-ftell(fp), fp);
	}
	fclose(fp);
	return result;
}

static object_struct add_obj(const char *filename, const char *texbmp)
{
	object_struct new_node;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, filename);

	if (!err.empty()||shapes.size()==0)
	{
		std::cerr<<err<<std::endl;
		exit(1);
	}

	glGenVertexArrays(1, &new_node.vao);
	glGenBuffers(4, new_node.vbo);
	glGenTextures(1, &new_node.texture);

	glBindVertexArray(new_node.vao);

	// Upload postion array
	glBindBuffer(GL_ARRAY_BUFFER, new_node.vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*shapes[0].mesh.positions.size(),
			shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	if(shapes[0].mesh.texcoords.size()>0)
	{

		// Upload texCoord array
		glBindBuffer(GL_ARRAY_BUFFER, new_node.vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*shapes[0].mesh.texcoords.size(),
				shapes[0].mesh.texcoords.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindTexture( GL_TEXTURE_2D, new_node.texture);
		unsigned int width, height;
		unsigned short int bits;
		unsigned char *bgr=load_bmp(texbmp, &width, &height, &bits);
		GLenum format = (bits == 24? GL_BGR: GL_BGRA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, bgr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
		delete [] bgr;
	}

	if(shapes[0].mesh.normals.size()>0)
	{
		// Upload normal array
		glBindBuffer(GL_ARRAY_BUFFER, new_node.vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*shapes[0].mesh.normals.size(),
				shapes[0].mesh.normals.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Setup index buffer for glDrawElements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_node.vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*shapes[0].mesh.indices.size(),
			shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

	new_node.indiceCount = shapes[0].mesh.indices.size();

	glBindVertexArray(0);
	return new_node;
}

void changeSize(int w, int h) {
    glViewport(0, 0, w, h);
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
	static object_struct sun = add_obj("sun.obj", "sun.bmp");
	static object_struct square = add_obj("square.obj", "texture.bmp");
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

	glBindVertexArray(square.vao);
	glBindTexture(GL_TEXTURE_2D, square.texture);
	glDrawElements(GL_TRIANGLES, square.indiceCount, GL_UNSIGNED_INT, nullptr);

	modelMat = glm::scale(modelMat, glm::vec3(0.5f, 0.5f, 0.5f));
	modelMat = glm::translate(modelMat, glm::vec3(5.0f, 0.0f, 5.0f));
	setUniformMat4(programs[currentKey], "modelMatrix", modelMat);
	glDrawElements(GL_TRIANGLES, square.indiceCount, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
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
    GLuint v = glCreateShader(GL_VERTEX_SHADER);
    GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
    char* vertShader = getTxtFile(vertexFile);
    char* fragShader = getTxtFile(fragmentFile);
    glShaderSource(v, 1, (const GLchar**)&vertShader, NULL);
    glShaderSource(f, 1, (const GLchar**)&fragShader, NULL);
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
	glewExperimental = GL_TRUE;
    glewInit();
    if (glewIsSupported("GL_VERSION_3_3")){
        printf("OpenGL 3.3 is supported\n");
	}else{
        printf("OpenGL 3.3 not supported\n");
        exit(1);
    }
    glEnable(GL_DEPTH_TEST);

	// black background
    glClearColor(0.2, 0.2, 0.2, 1.0);
	programs['a'] = initShaders("vertexA.txt", "fragA.txt");
	programs['p'] = initShaders("vertexP.txt", "fragP.txt");
	programs['g'] = initShaders("vertexG.txt", "fragG.txt");
	programs['A'] = programs['a'];
	programs['P'] = programs['p'];
	programs['G'] = programs['g'];
    glutMainLoop();

    return(0); 
}
