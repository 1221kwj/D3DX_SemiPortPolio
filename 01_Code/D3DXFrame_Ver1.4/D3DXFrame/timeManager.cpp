#include "stdafx.h"
#include "timeManager.h"

timeManager::timeManager()
	: _timer	( nullptr )
	, isRender	( true )
	, font		( nullptr )
	, color		( D3DCOLOR_XRGB( 255, 255, 255 ) )
	, textFormat( DT_LEFT | DT_TOP | DT_NOCLIP )
{
}

timeManager::~timeManager() {}

HRESULT timeManager::init()
{
	_timer = new timer;
	_timer->init();

	color = D3DCOLOR_XRGB( 0, 0, 0 );
	font = FONTMANAGER->GetFont( fontManager::NORMAL );

	return S_OK;
}

void timeManager::release()
{
	if ( _timer != NULL )
	{
		SAFE_DELETE( _timer );
		_timer = NULL;
	}

	SAFE_RELEASE( font );
}

void timeManager::update( float lock )
{
	if ( _timer != NULL )
	{
		_timer->tick( lock );
	}
}

void timeManager::render()
{
	if (isRender )
	{
		char str[ 256 ];
		string frameRate;
		RECT RC_FPS, RC_WORLD, RC_ELPASED;
		SetRect( &RC_FPS,		0,  0, 100, 20);
		SetRect( &RC_WORLD,		0, 20, 100, 40);
		SetRect( &RC_ELPASED,	0, 40, 100, 60);

#ifdef _DEBUG
		{
			//FPS
			sprintf_s( str, "FramePerSec(FPS) : %2d", _timer->getFrameRate() );
			font->DrawTextA( nullptr, str, 256, &RC_FPS, textFormat, color );

			sprintf_s( str, "WorldTime        : %5.5f", _timer->getWorldTime() );
			font->DrawTextA( nullptr, str, 256, &RC_WORLD, textFormat, color );

			sprintf_s( str, "ElaspedTime      : %2.4f", _timer->getElapsedTime() );
			font->DrawTextA( nullptr, str, 256, &RC_ELPASED, textFormat, color );
		}
#else
		{
			//FPS
			sprintf_s( str, "FramePerSec(FPS) : %2d", _timer->getFrameRate() );
			font->DrawTextA( nullptr, str, 256, &RC_FPS, textFormat, color );
		}
#endif

	}
	
}
