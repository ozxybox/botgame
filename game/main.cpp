#define SDL_MAIN_HANDLED
#include <SDL2/include/SDL.h>
#include <stdio.h>
#include <glad/include/glad/glad.h>
#include "gameinfo.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <time.h>
#include "transform.h"
#include "log.h"
#include <shadermanager.h>
#include <renderer.h>
#include <primitivedraw.h>


static SDL_Window* window;


bool initWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		Log::Fatal("Failed to init SDL2\n");
		return 1;
	}

	SDL_GL_LoadLibrary(NULL);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	window = SDL_CreateWindow(
		GAME_NAME,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 800, SDL_WINDOW_OPENGL
	);
	if (!window)
	{
		Log::Fatal("Failed to create window\n");
		return 2;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (!glContext)
	{
		Log::Fatal("Failed to create GL context\n");
		return 3;
	}

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	return 0;
}

int main()
{
	if (initWindow())
		return 1;
	Log::Msg("Made the window\n");

	// Enable depth test
	//glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	CShaderManager sm;
	CRenderer rm;
	CPrimitiveDraw pd;

	GLuint programID = ShaderManager().GetProgram("cube.vs", "cube.fs");


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, -3), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View;// *model.Matrix(); // Remember, matrix multiplication is the other way around


	Log::Msg("ready\n");
	GLuint mvpUniform = glGetUniformLocation(programID, "u_mvp");

	bool run = true;
	SDL_Event e;
	while (run)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				run = false;
			}
		}

		View = glm::lookAt(
			glm::vec3(4, 3 * cos(clock()/1000.0f), -3), // Camera is at (4,3,-3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		MVP = Projection * View;// *model.Matrix(); // Remember, matrix multiplication is the other way around

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &MVP[0][0]);

		CTransform t;
		PrimitiveDraw().DrawCube(t);
		


		SDL_GL_SwapWindow(window);
	}

	
	glDeleteProgram(programID);

	return 0;
}