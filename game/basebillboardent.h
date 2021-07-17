#pragma once
#include "baseentity.h"
#include "shaders.h"
#include "textures.h"


class CBaseBillboardEnt : public CBaseEntity
{
public:
	CBaseBillboardEnt(const char* classname) : CBaseEntity(classname) {}
	void SetTileset(const char* path, unsigned int tilew, unsigned int tileh);
	virtual void Render();
	void SetTilePos(int x, int y) { m_tileX = x;  m_tileY = y; }
protected:
	shader_t m_shader;
	tileset_t m_tileset;
	int m_tileX;
	int m_tileY;

};