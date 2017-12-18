#include "stdafx.h"
#include "AutoReleasePool.h"

AutoReleasePool:: AutoReleasePool() {}
AutoReleasePool::~AutoReleasePool() {}

void AutoReleasePool::AddObject( Object * object )
{
	objectList.push_back( object );
}

void AutoReleasePool::Drain()
{
	for each( auto p in objectList ) SAFE_RELEASE( p );
	objectList.clear();
}
