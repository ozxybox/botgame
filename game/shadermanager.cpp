#include "shadermanager.h"
#include <glad/glad.h>
#include <stdio.h>
#include <cstdlib>
#include "log.h"

DEFINE_SINGLETON(ShaderManager);


void loadShader(const char* path, GLuint shaderID)
{
	// Read File
	FILE* f = fopen(path, "rb");
	if (!f)
	{
		Log::Fault("Failed to load shader %s\n", path);
		return;
	}
	fseek(f, 0, SEEK_END);
	size_t len = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* str = (char*)malloc(len);
	fread(str, 1, len, f);

	// Pass it into gl
	glShaderSource(shaderID, 1, &str, NULL);
	glCompileShader(shaderID);

	GLint result = GL_FALSE;
	GLsizei size = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &size);
	if (size > 0) {
		char* log = (char*)malloc(size);
		glGetShaderInfoLog(shaderID, size, NULL, log);
		Log::Fault("%s\n", log);
		free(log);
	
	}
	else
	{
		Log::Msg("Loaded Shader %s\n", path);
	}

	free(str);
	fclose(f);
}


CShaderManager::CShaderManager()
{
	s_singleton = this;
}

CShaderManager::~CShaderManager()
{
	s_singleton = nullptr;
}

shader_t CShaderManager::GetProgram(const char* vertex, const char* fragment)
{
	shader_t vs = GetShader(vertex, GL_VERTEX_SHADER);
	shader_t fs = GetShader(fragment, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	return program;
}

shader_t CShaderManager::GetShader(const char* path, unsigned int type)
{
	std::string spath = path;
	shader_t shader;
	if (m_shaders.contains(spath))
		shader = m_shaders[spath];
	else
	{
		shader = glCreateShader(type);
		loadShader(path, shader);
		m_shaders.emplace(spath, shader);
	}

	return shader;
}

void CShaderManager::BindShader(shader_t shader)
{
}
