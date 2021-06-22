#pragma once

#define DECLARE_SYSTEM(classname) \
	typedef C##classname ThisClass;

#define DECLARE_SINGLETON(classname) \
	DECLARE_SYSTEM(classname)        \
	friend ThisClass& classname();   \

#define DECLARE_SINGLETON_ACCESSOR(classname) \
	class C##classname; \
	C##classname& classname();

#define DEFINE_SINGLETON(classname) \
	C##classname* IBaseSingleton<C##classname>::s_singleton = nullptr; \
	C##classname& classname() { return *(C##classname*)C##classname::s_singleton; } 

/*
#define SINGLETON_CLASS(classname)       \
	DECLARE_SINGLETON_ACCESSOR(classname);   \
	class C##classname : public IBaseSingleton<C##classname>
*/


class IBaseSystem
{
};

template<typename ClassName>
class IBaseSingleton : public IBaseSystem
{
public:
	 IBaseSingleton() {
		 s_singleton = (ClassName*)this;
	 }
	~IBaseSingleton() {
		s_singleton = nullptr;
	}

protected:
	static ClassName* s_singleton;
};
