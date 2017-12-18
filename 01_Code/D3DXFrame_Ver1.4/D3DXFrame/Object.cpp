#include "stdafx.h"
#include "Object.h"

Object::Object( void )
	: refCount( 1 )
{
	OBJECTMANAGER->AddObject( this );
}

Object::~Object( void )
{
	OBJECTMANAGER->RemoveObject( this );
	assert( refCount <= 0 && "Release를 이용해서 객체를 해제하세요." );
}

void Object::AddRef()
{
	++refCount;
}

void Object::Release()
{
	--refCount;
	if ( refCount <= 0 ) delete this;
}

void Object::AutoRelease()
{
	AUTORELEASEPOOL->AddObject( this );
}
