#pragma once
#include "gameNode.h"

class MainGameManager : public gameNode
{
private:
	bool WireFrameOn;

public:
	MainGameManager();
	virtual ~MainGameManager();

	virtual HRESULT init( void );
	virtual void release( void );
	virtual void update( void );
	virtual void render( void );
	//virtual void MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

