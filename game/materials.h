#pragma once
#include "basesystem.h"
#include "shaders.h"
#include "textures.h"

struct material_t
{
	shader_t shader;
	texture_t texture;
};

DECLARE_SINGLETON_ACCESSOR(Materials);
class CMaterials : public IBaseSingleton<CMaterials>
{
	DECLARE_SINGLETON(Materials);
public:


};
