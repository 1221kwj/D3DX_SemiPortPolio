#pragma once
#include "singletonBase.h"
#include "timer.h"

class timeManager : public singletonBase<timeManager>
{
private:
	timer*			_timer;
	LPD3DXFONT		font;
	D3DCOLOR		color;
	DWORD			textFormat;

public:
	timeManager();
	~timeManager();

	HRESULT init();
	void release();
	void update( float lock = 0.0f );
	void render();

	inline float getElapsedTime( void ) const { return _timer->getElapsedTime(); }
	inline float getWorldTime( void )	const { return _timer->getWorldTime(); }

	GETSET( bool, isRender, IsRender );
};

