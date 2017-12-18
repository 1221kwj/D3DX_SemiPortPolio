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
	assert( refCount <= 0 && "Release�� �̿��ؼ� ��ü�� �����ϼ���." );
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
