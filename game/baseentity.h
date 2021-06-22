#pragma once
#include <map>
#include <string>
#include <vector>
#include "transform.h"

class CBaseEntity : public CTransform
{
public:
	CBaseEntity(const char* classname);
	
	void Spawn();

	// Gets called on removal. The ent will no longer exist next frame
	virtual void Removed();

	// Deletes this entity
	void RemoveThis();

	virtual void Render(){};
	virtual void Update(){};


private:
	bool m_live;
	const char* m_classname;
	friend class CEntityManager;
};



class IEntityFactory;

class CEntityManager
{
public:
	void RegisterFactory(IEntityFactory* factory);
	CBaseEntity* CreateEntity(char* name);
	CBaseEntity* GetEnt(const char* name);
	void Update();
	void Render();
	void Remove(CBaseEntity* ent);
	void ClearAll();
private:
	std::map<std::string, IEntityFactory*> m_factories;
	std::vector<CBaseEntity*> m_ents;
	std::vector<int> m_removeNextFrame;
};

CEntityManager& EntManager();

class IEntityFactory
{
public:
	virtual const char* ClassName() = 0;
	virtual CBaseEntity* CreateEntity() = 0;
};

template<typename T>
class CEntityFactory : public IEntityFactory
{
public:
	CEntityFactory<T>(const char* classname)
	{
		m_name = classname;
		EntManager().RegisterFactory(this);
	}
	virtual const char* ClassName() { return m_name; }
	virtual CBaseEntity* CreateEntity() { return new T(m_name); }

	const char* m_name;
};

#define DECLARE_ENT(classname, cppname) static CEntityFactory<cppname> s_factory##cppname##classname(#classname); 
