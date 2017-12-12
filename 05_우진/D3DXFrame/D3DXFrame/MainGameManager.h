#pragma once
#include "gameNode.h"

class MainGameManager : public gameNode
{
private:

public:
	MainGameManager();
	virtual ~MainGameManager();

	virtual HRESULT init( void );
	virtual void release( void );
	virtual void update( void );
	virtual void render( void );
};

