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

	shader_t GetProgram(const char* vertex, const char* fragment);

private:
	shader_t GetShader(const char* path, unsigned int type);
	
	std::map<std::string, shader_t> m_shaders;
	std::map<std::string, shader_t> m_programs;
};

