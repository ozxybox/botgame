#include "baseentity.h"
#include <cstring>

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
	ent->Spawn();
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
	
	// Fix it so we don't update fresh ents this frame.
	int len = m_ents.size();
	for (int i = 0; i < len; i++)
		m_ents[i]->Update();
}

void CEntityManager::Render()
{

	// Fix it so we don't render fresh ents this frame.
	int len = m_ents.size();
	for (int i = 0; i < len; i++)
		m_ents[i]->Render();

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
