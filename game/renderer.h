#pragma once

#include "basesystem.h"

DECLARE_SINGLETON_ACCESSOR(Renderer);
class CRenderer : public IBaseSingleton<CRenderer>
{
	DECLARE_SINGLETON(Renderer);
public:
	CRenderer();
	~CRenderer();

};

