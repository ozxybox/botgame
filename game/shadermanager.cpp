#include "shadermanager.h"
#include <glad/glad.h>
#include <stdio.h>
#include <cstdlib>
#include "log.h"
#include <renderer.h>

DEFINE_SINGLETON(ShaderManager);

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
	for(auto p : m_programs)
		glDeleteProgram(p.second);

	for (auto p : m_shaders)
		glDeleteShader(p.second);
}

shader_t CShaderManager::GetProgram(const char* vertex, const char* fragment)
{
	std::string programName = std::string(vertex) + std::string(fragment);
	if (m_programs.contains(programName))
		return m_programs[programName];

	shader_t vs = GetShader(vertex,   GL_VERTEX_SHADER);
	shader_t fs = GetShader(fragment, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	m_programs.emplace(programName, program);
	return program;
}

shader_t CShaderManager::CreateProgramFromText(const char* vertex, const char* fragment)
{
	// Make up fake names
	static int s_fromTextCount = 0;
	char vertexName[64];
	char fragmentName[64];

	sprintf(vertexName,   "|__sm_vs_pft_%d", s_fromTextCount);
	sprintf(fragmentName, "|__sm_fs_pft_%d", s_fromTextCount);
	s_fromTextCount++;
	
	std::string programName = std::string(vertexName) + std::string(fragmentName);

	GLuint vs = CreateShaderFromText(vertexName,   vertex,   GL_VERTEX_SHADER);
	GLuint fs = CreateShaderFromText(fragmentName, fragment, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	m_programs.emplace(programName, program);

	return program;
}

shader_t CShaderManager::GetShader(const char* path, unsigned int type)
{
	std::string spath = path;
	shader_t shader;
	if (m_shaders.contains(spath))
		shader = m_shaders[spath];
	else
		shader = LoadShaderFromFile(path, type);

	return shader;
}

shader_t CShaderManager::CreateShaderFromText(const char* name, const char* str, unsigned int type)
{
	GLuint shaderID = glCreateShader(type);
	
	// Pass it into gl
	glShaderSource(shaderID, 1, &str, NULL);
	glCompileShader(shaderID);

	// Check for errors
	GLint result = GL_FALSE;
	GLsizei size = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &size);
	if (size > 0) {
		char* log = (char*)malloc(size);
		glGetShaderInfoLog(shaderID, size, NULL, log);
		Log::Fault("Failed to load shader %s!\n%s\n--File--\n%s\n", name, log, str);
		free(log);
	}
	else
	{
		Log::Msg("Loaded Shader %s\n", name);
	}

	// Register to cache
	m_shaders.emplace(name, shaderID);
	return shaderID;
}

shader_t CShaderManager::LoadShaderFromFile(const char* path, unsigned int type)
{
	// Read File
	FILE* f = fopen(path, "r");
	if (!f)
	{
		Log::Fault("Failed to load shader %s\n", path);
		return GL_INVALID_INDEX;
	}
	fseek(f, 0, SEEK_END);
	size_t len = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* str = (char*)calloc(len, 1);
	fread(str, 1, len, f);

	shader_t shader = CreateShaderFromText(path, str, type);

	free(str);
	fclose(f);

	return shader;
}
