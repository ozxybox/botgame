#include "world.h"
#include "cell.h"
#include "log.h"

DEFINE_SINGLETON(World);
DECLARE_ENT(CWorld, world);

CWorld::CWorld(const char* classname) : IBaseSingleton<CWorld>(), CBaseEntity(classname)
{

}

void CWorld::CreateWorld()
{
}

void CWorld::AddTarget(CTransform* target, int range)
{
	m_targets.push_back({ target,range });
}

bool TransformInCell(CTransform* target, CCell* cell)
{
	glm::vec3 t = target->GetAbsOrigin();
	glm::vec3 c = cell->GetAbsOrigin();

	return t.x >= c.x && t.x < c.x + 10 && t.z >= c.z && t.z < c.z + 10;
}

void CWorld::Update()
{


	for (auto t : m_targets)
	{
		CTransform* target = t.target;
		glm::vec3 pos = target->GetAbsOrigin();
		bool inACell = false;
		for (auto c : m_cells)
		{
			if (TransformInCell(target, c))
			{
				inACell = true;
				break;
			}
		}
		if (!inACell)
		{
			CCell* cell = (CCell*)EntManager().CreateEntity("cell");
			cell->SetAbsOrigin(floor(pos.x / 10) * 10 , 0, floor(pos.z / 10) * 10);
			m_cells.push_back(cell);
			Log::Print("Made a new cell!\n");
		}
	}
}
