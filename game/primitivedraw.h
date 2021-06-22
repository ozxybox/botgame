#pragma once
#include "transform.h"
#include "basesystem.h"

DECLARE_SINGLETON_ACCESSOR(PrimitiveDraw);
class CPrimitiveDraw : public IBaseSingleton<CPrimitiveDraw>
{
	DECLARE_SINGLETON(PrimitiveDraw);
public:
	CPrimitiveDraw();
	~CPrimitiveDraw();

	void DrawCube(CTransform& transform);

private:

	unsigned int m_cubeVAO;
	unsigned int m_cubeVBO;
	unsigned int m_cubeIBO;

};
