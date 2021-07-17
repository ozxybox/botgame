#include "renderer.h"
#include "textures.h"

#include <SDL.h>
#include <log.h>
#include <gameinfo.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>


DEFINE_SINGLETON(Renderer);



SDL_Window* initWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		Log::Fatal("Failed to init SDL2\n");
		return 0;
	}

	SDL_GL_LoadLibrary(NULL);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_Window* window = SDL_CreateWindow(
		GAME_NAME,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 800, SDL_WINDOW_OPENGL
	);
	if (!window)
	{
		Log::Fatal("Failed to create window\n");
		return 0;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (!glContext)
	{
		Log::Fatal("Failed to create GL context\n");
		return 0;
	}

	gladLoadGLLoader(SDL_GL_GetProcAddress);
	
	return window;
}

CRenderer::CRenderer()
{
	m_window = initWindow();
	if (!m_window)
		return;

	Log::Msg("Made the window\n");

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

CRenderer::~CRenderer()
{
}

void CRenderer::ClearFrame(glm::vec3 color)
{
	glClearColor(color.r, color.g, color.b, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CRenderer::PushFrame()
{
	SDL_GL_SwapWindow((SDL_Window*)m_window);


}

void CRenderer::SetPerspective(float fov)
{
	int w, h;
	SDL_GetWindowSize((SDL_Window*)m_window, &w, &h);
	m_projection = glm::perspective(fov, w / (float)h, 0.1f, 100.0f);
}

void CRenderer::SetOrtho(float l, float r, float t, float b)
{
	m_projection = glm::ortho(l, r, b, t);
}

void CRenderer::SetMatrix(MatrixMode mode, glm::mat4x4 mat)
{
	switch (mode)
	{
	case MatrixMode::MODEL:
		m_model = mat;
		return;

	case MatrixMode::VIEW:
		m_view = mat;
		return;

	case MatrixMode::PROJECTION:
		m_projection = mat;
		return;
	}

}


void CRenderer::BindShader(shader_t shader)
{
	if (m_currentShader != shader)
	{
		glUseProgram(shader);
		m_currentShader = shader;
	}

	// Send over the uniforms
	glm::mat4x4 mvp = m_projection * m_view * m_model;
	GLuint mvpUniform = glGetUniformLocation(shader, "u_mvp");
	glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &mvp[0][0]);
	
}

void CRenderer::BindTexture(texture_t texture)
{
	glBindTexture(GL_TEXTURE_2D, Textures().RenderHandle(texture));
}

void CRenderer::GetWindowSize(int& w, int& h)
{
	if (SDL_GetWindowFlags((SDL_Window*)m_window) & SDL_WINDOW_MINIMIZED)
	{
		w = 0;
		h = 0;
	}
	else
	{
		SDL_GetWindowSize((SDL_Window*)m_window, &w, &h);
	}
}

void CRenderer::DrawObject(int vao, int vbo, int ibo, int elementCount)
{

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		0,                             // position 0
		3,                             // three floats
		GL_FLOAT,                      // elements are floats
		GL_FALSE,                      //
		sizeof(vertex_t),              // stride
		(void*)offsetof(vertex_t, pos) // position within stride
	);
	glVertexAttribPointer(
		1,                            // position 1
		2,                            // two floats
		GL_FLOAT,                     // elements are floats
		GL_FALSE,                     //
		sizeof(vertex_t),             // stride
		(void*)offsetof(vertex_t, uv) // position within stride
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);
}

void CRenderer::BindTile(tileset_t tileset, int x, int y)
{ 
	glm::vec4 rect = Textures().TileRect(tileset, x, y);
	GLuint subrectUniform = glGetUniformLocation(m_currentShader, "u_subrect");
	glUniform4fv(subrectUniform, 1, &rect.x);
	BindTexture(Textures().TilesetTexture(tileset));
}
