#pragma once
#include "transform.h"
#include "basesystem.h"
#include <shadermanager.h>

DECLARE_SINGLETON_ACCESSOR(PrimitiveDraw);
class CPrimitiveDraw : public IBaseSingleton<CPrimitiveDraw>
{
	DECLARE_SINGLETON(PrimitiveDraw);
public:
	CPrimitiveDraw();
	~CPrimitiveDraw();

	void DrawCube(CTransform& transform, shader_t shader);
	void DrawCube(CTransform& transform);

private:

	unsigned int m_cubeVAO;
	unsigned int m_cubeVBO;
	unsigned int m_cubeIBO;

	shader_t m_cubeShader;
};
