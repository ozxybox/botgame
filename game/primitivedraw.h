#pragma once
#include "transform.h"
#include "basesystem.h"
#include "shaders.h"

DECLARE_SINGLETON_ACCESSOR(PrimitiveDraw);
class CPrimitiveDraw : public IBaseSingleton<CPrimitiveDraw>
{
	DECLARE_SINGLETON(PrimitiveDraw);
public:
	CPrimitiveDraw();
	~CPrimitiveDraw();

	void DrawCube(CTransform& transform, shader_t shader);
	void DrawCube(CTransform& transform);


	void DrawQuad(CTransform& transform, shader_t shader);
	void DrawQuad(CTransform& transform);

private:
	unsigned int m_cubeVAO;
	unsigned int m_cubeVBO;
	unsigned int m_cubeIBO;


	unsigned int m_quadVAO;
	unsigned int m_quadVBO;
	unsigned int m_quadIBO;

	shader_t m_cubeShader;
};
