#include "stdafx.h"
#include "deviceManager.h"

deviceManager::deviceManager()
	: vd( nullptr )
{
}

deviceManager::~deviceManager() {}

void deviceManager::init( void )
{
	vd = new VideoDevice;
	vd->init();
}

void deviceManager::release( void )
{
	vd->release();
	SAFE_DELETE( vd );
}

void deviceManager::renderBegin( void )
{
	vd->GetDevice()->Clear(
		0,
		nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 47, 121, 112 ),
		1.0f,
		0
	);
	vd->GetDevice()->BeginScene();
}

void deviceManager::renderEnd( void )
{
	vd->GetDevice()->EndScene();
	vd->GetDevice()->Present( nullptr, nullptr, nullptr, nullptr );
}

LPDIRECT3DDEVICE9 deviceManager::GetDevice()
{
	return vd->GetDevice();
}

