#pragma once
#include "singletonBase.h"

class AutoReleasePool : public singletonBase <AutoReleasePool>
{
private:
	list<Object*> objectList;

public:
	AutoReleasePool();
	~AutoReleasePool();

	void AddObject( Object* object );
	void Drain();
};

