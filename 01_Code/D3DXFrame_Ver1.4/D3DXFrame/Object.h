#pragma once

class Object
{
private:
	int refCount;

public:
	Object();
	virtual ~Object();

	virtual void AddRef();
	virtual void Release();
	virtual void AutoRelease();
};

