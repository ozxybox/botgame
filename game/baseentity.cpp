#include "baseentity.h"

CBaseEntity::CBaseEntity(const char* classname)
{
	m_live = false;
	m_classname = classname;
}

void CBaseEntity::Spawn()
{
	m_live = true;
}

void CBaseEntity::Removed()
{
}

void CBaseEntity::RemoveThis()
{
	EntManager().Remove(this);
	m_live = false;
}


CEntityManager& EntManager()
{
	static CEntityManager s_CEntityManager;
	return s_CEntityManager;
}

void CEntityManager::RegisterFactory(IEntityFactory* factory)
{
	m_factories.insert({ std::string(factory->ClassName()), factory });
}

CBaseEntity* CEntityManager::CreateEntity(const char* name)
{
	if (m_factories.find(std::string(name)) == m_factories.end())
		return 0;

	CBaseEntity* ent = m_factories[name]->CreateEntity();
	m_ents.push_back(ent);
	return ent;
}

void CEntityManager::Update()
{
	for (auto i : m_removeNextFrame)
	{
		auto target = m_ents.begin() + i;
		delete (*target);
		m_ents.erase(target);
	}
	m_removeNextFrame.clear();
	for (auto e : m_ents)
		e->Update();
}

void CEntityManager::Render()
{
	for (auto e : m_ents)
		e->Render();

}

void CEntityManager::Remove(CBaseEntity* ent)
{
	auto f = std::find(m_ents.begin(), m_ents.end(), ent);
	if (f != m_ents.end())
	{
		m_removeNextFrame.push_back(f - m_ents.begin());
		(*f)->Removed();
	}
}

void CEntityManager::ClearAll()
{
	for (auto e : m_ents)
		delete e;
	m_ents.clear();
}

CBaseEntity* CEntityManager::GetEnt(const char* name)
{
	for (auto e : m_ents)
		if (strcmp(e->m_classname, name) == 0)
		return e;
	return nullptr;
}
