#pragma once
#include "singletonBase.h"

class ObjectManager : public singletonBase <ObjectManager>
{
private:
	set<Object*> m_setObject;

public:
	ObjectManager();
	~ObjectManager();

	void AddObject( Object* object );
	void RemoveObject( Object* object );
	void Destroy();
};

