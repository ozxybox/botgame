#pragma once
#include "baseentity.h"

// 10x10 tile grid section
// Cell Origins are based on their top right corner; a Cell from 0,0 will extend to 9,9. Each tile is 1x1, so a tile at 9,9 will end at 10,10
class CCell : public CBaseEntity
{
public:
	CCell(const char* classname);

	virtual void Render();

protected:
};
