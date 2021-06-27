#pragma once
#include <map>
#include <string>
#include "basesystem.h"

typedef unsigned int shader_t;


DECLARE_SINGLETON_ACCESSOR(ShaderManager);
class CShaderManager : public IBaseSingleton<CShaderManager>
{
	DECLARE_SINGLETON(ShaderManager)

public:
	CShaderManager();
	~CShaderManager();

	// Gets a program by loading vertex and fragment shader files
	shader_t GetProgram(const char* vertex, const char* fragment);
	// Creates a program based on supplied text
	shader_t CreateProgramFromText(const char* vertex, const char* fragment);

private:
	// Checks cache if the shader exists, if it does, it returns it, else it loads it from file.
	shader_t GetShader(const char* path, unsigned int type);

	// Creates the shader. Does not check cache. Registers to cache
	shader_t CreateShaderFromText(const char* name, const char* str, unsigned int type);

	// Creates the shader from file, does not check cache. Registers to cache
	shader_t LoadShaderFromFile(const char* path, unsigned int type);

	std::map<std::string, shader_t> m_shaders;
	std::map<std::string, shader_t> m_programs;
};

