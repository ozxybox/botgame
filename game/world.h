#pragma once
#include "basesystem.h"
#include "baseentity.h"

class CCell;

DECLARE_SINGLETON_ACCESSOR(World);
class CWorld : public CBaseEntity, public IBaseSingleton<CWorld>
{
	DECLARE_SINGLETON(World);
public:
	CWorld(const char* classname);
	void CreateWorld();
	// Keeps everything within range of the target loaded
	void AddTarget(CTransform* target, int range);
	virtual void Update();

private:
	struct target_t { CTransform* target; int range; };
	std::vector<target_t> m_targets;
	std::vector<CCell*> m_cells;

};