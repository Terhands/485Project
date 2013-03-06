#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "Pipeline.h"
#include "camera.h"
#include "model.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

using namespace std;

void initWindow(int argc, char** argv);
void initCallbacks();
void loadTextures();

static void keyboardReg(unsigned char key, int x, int y);
static void keyboardSpec(int key, int x, int y);
static void mouseHandler(int button, int state, int x, int y);
void resize(int w, int h);

void RenderScene();
void createVertexBuffer();
void createIndexBuffer();

void CompileShaders();

void cleanup();

bool isDefault;

// the index buffer object address
GLuint IBO;

// the texture buffer object address?
GLuint TBO;

// the vertex buffer object address
GLuint VBO;
GLuint gWorldLocation;
GLuint gSampler;

/* the current width & height of the screen */
GLuint screen_width;
GLuint screen_height;

GLuint* textures;

/* the camera object controls its location & view direction */
Camera camera;

/* the model to be displayed */
Model model;

/* the triangle indices & the vertex definitions for the model */
unsigned int* indices;
Vertex* vertices;

/* simple vertex shader takes in a position & modelView matrix from
the main program and uses clamp to vary the colour of each fragment 
(Color gets passed on to the fragment shader) */
static const char* VertexShader = "											\n\
#version 330							 									\n\
																			\n\
layout (location = 0) in vec3 Position;										\n\
layout (location = 1) in vec2 inTexCoord;									\n\
																			\n\
uniform mat4 modelView;														\n\
																			\n\
out vec4 Color;																\n\
out vec2 TextureCoords;														\n\
																			\n\
void main()																	\n\
{																			\n\
	gl_Position = modelView * vec4(Position, 1.0);							\n\
	Color = vec4(0.5, 0.0, 0.8, 1.0);										\n\
	TextureCoords = inTexCoord;												\n\
}";

/* simple shader that gets a color from the vertex shader and just passes it along */
static const char* FragmentShader = "										\n\
#version 330																\n\
																			\n\
out vec4 FragColor;															\n\
																			\n\
uniform sampler2D gSampler;													\n\
																			\n\
in vec4 Color;																\n\
in vec2 TextureCoords;														\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = texture2D(gSampler, TextureCoords.xy);						\n\
}";


int main(int argc, char** argv)
{
	ModelReader reader;
	reader.ReadModel("model.txt", model);

	initWindow(argc, argv);
	initCallbacks();

	GLenum result = glewInit();
	if(result != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(result));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	createVertexBuffer();
	createIndexBuffer();
	CompileShaders();

	glUniform1i(gSampler, 0);
	loadTextures();

	glutMainLoop();

	return 0;
}

/* set up the display window */
void initWindow(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100,100);
	glutCreateWindow(model.getBuildingName());

	screen_width = SCREEN_WIDTH;
	screen_height = SCREEN_HEIGHT;
}

/* set up all of the event callback functions */
void initCallbacks()
{
	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	glutKeyboardFunc(keyboardReg);
	glutSpecialFunc(keyboardSpec);
	glutMouseFunc(mouseHandler);
	glutReshapeFunc(resize);
}

/* main draw function to render the model from the perspective of the camera */
void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	/* setting up the transformation pipeline */
	Pipeline p;
	/* start out looking along the z axis, the camera defaults to start looking along z from the world's origin */
	p.WorldPos(-2.0f, -2.0f, 20.0f);
	p.Rotate(0.0f, 0.0f, 0.0f);
	p.SetCamera(camera.getCameraPosition(), camera.getViewTarget(), camera.getUpDirection());
	p.SetPerspectiveProjection(45.0f, screen_width, screen_height, 1.0f, 1000.0f);

	/* the camera is generating a rotation matrix relative to the model world */
	Matrix4f m = (*(p.GetTrans()) * camera.getRotationM());

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)&m);

	if(!isDefault)
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		/* setting up the vertex buffer so the positions are accessible to the shader */
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)sizeof(Vector3f));

		/* setting up the index buffer so the shader can use it to figure out which points belong to which triangle */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);

		glDrawElements(GL_TRIANGLES, model.numTriangles() * 3, GL_UNSIGNED_INT, 0);

		/*
		for(int i = 0; i < model.numTriangles() * 3; i += 3)
		{
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &indices[i]);
		}
		*/

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
	else
	{
		glutSolidTeapot(1.0f);
	}

	/* the text should always stay in the same location */
	Matrix4f m_idenity = getIdentity();

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)&m_idenity);

	char* name = "Teresa Hume, tch562 (10152929)";

	glRasterPos2f(0.46,-0.95);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i<30; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, name[i]);
	}

	/* swap the front and back buffers so the rendered image moves smoothly */
	glutSwapBuffers();
}

/* the change in aspect ratio should get handled in the draw function where the perspective projection is being set up */
void resize(int w, int h)
{
	screen_width = w;
	screen_height = h;
	
	glViewport(0,0,screen_width, screen_height);
	
	//glutPostRedisplay();
}

/* function to build the vertex array for the triangles to be drawn */
void createVertexBuffer()
{
	/* set the triangle points */
	int num_vertices = model.numVertices();
	vertices = new Vertex[num_vertices];
	model.getVertices(vertices);

	/* add the vertices to the buffer so the points can be read by the vertex shader */
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * num_vertices, vertices, GL_STATIC_DRAW);
}

/* set up the index buffer (allows sharing common points from the vertex array) */
void createIndexBuffer()
{
	/* defining the triangle index ordering in the vertex array */
	int num_indices = model.numTriangles() * 3;
	indices = new unsigned int[num_indices];
	model.getTriangles(indices);

	/* add the index array to the index buffer so it is accessible to the vertex shader */
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*indices) * num_indices, indices, GL_STATIC_DRAW);
}

void loadTextures()
{
	// not sure if this is working... each index of textures has the same value?
	textures = new GLuint[model.numTextures()];
	glGenTextures(model.numTextures(), textures);

	for(int i = 0; i < 10; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, model.getTexture(i)->width, model.getTexture(i)->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, model.getTexture(i)->texture);

		// setting how the texture will be mapped if the triangle is larger or smaller than the texture image
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

static void keyboardReg(unsigned char key, int x, int y)
{
	camera.keyboardHandler(key);

	// on escape free up memory and exit the program
	if (key == 27)
	{
		cleanup();
	}
}

static void keyboardSpec(int key, int x, int y)
{
	camera.keyboardSpecial(key);
}

static void mouseHandler(int button, int state, int x, int y)
{
	camera.mouseHandler(button);
}

/* read in, compile, and link the shaders so they can run on the GPU*/
void AddShader(GLuint shaderProgram, const char* shaderText, GLenum shaderType)
{
	GLuint ShaderObj = glCreateShader(shaderType);

	if(ShaderObj == 0)
	{
		fprintf(stderr, "Error creating the shader type %d\n", shaderType);
		exit(1);
	}

	const GLchar* p[1];
	p[0] = shaderText;
	GLint lengths[1];
	lengths[0] = strlen(shaderText);
	glShaderSource(ShaderObj, 1, p, lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
		exit(1);
	}

	glAttachShader(shaderProgram, ShaderObj);
}

void CompileShaders()
{
	GLuint shaderProgram = glCreateProgram();

	if(shaderProgram == 0)
	{
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	AddShader(shaderProgram, VertexShader, GL_VERTEX_SHADER);
	AddShader(shaderProgram, FragmentShader, GL_FRAGMENT_SHADER);

	GLint success = 0;
	GLchar errorLog[1024] = { 0 };

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(success == 0)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}

	glUseProgram(shaderProgram);

	gWorldLocation = glGetUniformLocation(shaderProgram, "modelView");
	assert(gWorldLocation != 0xFFFFFFFF);

	gSampler = glGetUniformLocation(shaderProgram, "gSampler");
	assert(gSampler != 0xFFFFFFFF);
}

void cleanup()
{
	if (indices != NULL) { delete [] indices; indices = NULL; }
	if (vertices != NULL) {delete [] vertices; vertices = NULL; }

	exit(0);
}