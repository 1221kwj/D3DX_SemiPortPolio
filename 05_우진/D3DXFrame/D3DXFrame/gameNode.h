#pragma once

class gameNode
{
private:
	bool _managerInit;

public:
	gameNode();
	virtual ~gameNode();

	virtual HRESULT init( void );
	virtual HRESULT init( bool managerInit );
	virtual void release( void );
	virtual void update( void );
	virtual void render( void );

	LRESULT MainProc( HWND, UINT, WPARAM, LPARAM );
};

