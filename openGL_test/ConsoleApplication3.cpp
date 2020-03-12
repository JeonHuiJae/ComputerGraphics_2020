#include "stdafx.h"
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/wglew.h>

using namespace std;

GLuint g_programID;
GLuint Buffer;
float r, g, b;

vector<float> vertices = {
};

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	//create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	//Read the vertex shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);
	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	//Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	//Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength != 0)
	{
		vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}

	//Read the fragment shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);
	if (FragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	//Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	//Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength != 0)
	{
		vector<char> FragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
	}
	//Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


void renderScene(void)
{
	GLint posAttrib = glGetAttribLocation(g_programID, "pos");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);
	glEnableVertexAttribArray(posAttrib); // active

	GLint colorAttrib = glGetAttribLocation(g_programID, "a_Color");
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(colorAttrib); // active

	//Clear all pixels
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Let's draw something here

	glDrawArrays(GL_LINES, 0, vertices.size()/6); // mode, start, count
	//Double buffer
	glutSwapBuffers();
}

void setNewColor() {
	r = (rand() % 100) / 99.0f;
	g = (rand() % 100) / 99.0f;
	b = (rand() % 100) / 99.0f;
}

void myMouseFunc(int btn, int state, int x, int y) {
	if (state == GLUT_DOWN && btn == GLUT_LEFT_BUTTON) {
		float nx = 2.0f * (float)x / (float)479 - 1.0f; //mapping to world coordinate
		float ny = -2.0f * (float)y / (float)479 + 1.0f;
		if (vertices.size() % 12 == 0)
			setNewColor();

		vertices.push_back(nx);//vertices
		vertices.push_back(ny);
		vertices.push_back(0.0f);
		vertices.push_back(r);//color
		vertices.push_back(g);
		vertices.push_back(b);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW); // data update
		glutPostRedisplay();//re-draw
	}
}

void myKeyFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 's':
		setNewColor();
		vertices[vertices.size() - 3] = r;
		vertices[vertices.size() - 2] = g;
		vertices[vertices.size() - 1] = b;
		break;
	case 'e':
		setNewColor();
		vertices[vertices.size() - 9] = r;
		vertices[vertices.size() - 8] = g;
		vertices[vertices.size() - 7] = b;
		break;
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW); // data update
	glutPostRedisplay();// re-draw
}

void init()
{
	GLenum res = glewInit();
	if(res != GLEW_OK)
		fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glLineWidth(30.0f);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_FUNC);
	glDepthFunc(GL_LESS);
	glDepthRange(0.5, 1.0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitWindowPosition(200, 200);
	glutInitWindowSize(480, 480);
	glutCreateWindow("hello OpenGL");
	init();

	//1.
	//Generate VAO
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//2.
	//Generate VBO
	glGenBuffers(1, &Buffer); // gen
	glBindBuffer(GL_ARRAY_BUFFER, Buffer); // bind
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW); // data
	
	//3. 
	g_programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
	glUseProgram(g_programID);

	//Set Function
	glutMouseFunc(myMouseFunc);
	glutKeyboardFunc(myKeyFunc);
	glutDisplayFunc(renderScene);

	//enter GLUT event processing cycle
	glutMainLoop();

	glDeleteVertexArrays(1, &VertexArrayID);
	return 1;
}