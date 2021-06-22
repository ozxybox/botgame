#pragma once
#include <map>
#include <string>
#include "basesystem.h"

typedef unsigned int shader_t;


SINGLETON_CLASS(ShaderManager)
{
	DECLARE_SINGLETON(ShaderManager)

public:
	CShaderManager();
	~CShaderManager();

	shader_t GetProgram(const char* vertex, const char* fragment);
	void BindShader(shader_t shader);

private:
	shader_t GetShader(const char* path, unsigned int type);
	
	std::map<std::string, shader_t> m_shaders;
};

