#include "stdafx.h"
#include "gameNode.h"

gameNode::gameNode() {}
gameNode::~gameNode() {}

HRESULT gameNode::init( void )
{
	_managerInit = false;

	return S_OK;
}

HRESULT gameNode::init( bool managerInit )
{
	_managerInit = managerInit;

	if ( _managerInit )
	{
		//싱글톤 매니저 초기화 부분(Initialize)
		DEVICEMANAGER	->init();
		CAMERAMANAGER	->init();
		GRIDMANAGER		->init();
		TEXTUREMANAGER	->init();
		FONTMANAGER		->init();
		SCENEMANAGER	->init();
		KEYMANAGER		->init();
		TIMEMANAGER		->init();
	}

	return S_OK;
}

void gameNode::release( void )
{
	if ( _managerInit )
	{
		//싱글톤 매니저 해제하는 부분
		RANDOMFUNCTION		->releaseSingleton();
		SKINNEDMESHMANAGER	->releaseSingleton();
		OBJLOADERMANAGER	->releaseSingleton();
		AUTORELEASEPOOL		->releaseSingleton();
		OBJECTMANAGER		->releaseSingleton();
		TIMEMANAGER			->releaseSingleton();
		KEYMANAGER			->releaseSingleton();
		SCENEMANAGER		->releaseSingleton();
		FONTMANAGER			->releaseSingleton();
		TEXTUREMANAGER		->releaseSingleton();
		GRIDMANAGER			->releaseSingleton();
		CAMERAMANAGER		->releaseSingleton();
		DEVICEMANAGER		->releaseSingleton();
	}
}

void gameNode::update( void )
{
}

void gameNode::render( void )
{
}

LRESULT gameNode::MainProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	CAMERAMANAGER->WndProc( hWnd, iMessage, wParam, lParam );

	switch ( iMessage )
	{
		case WM_MOUSEMOVE:
			_ptMouse.x = static_cast< float >LOWORD( lParam );
			_ptMouse.y = static_cast< float >HIWORD( lParam );
		break;
		case WM_LBUTTONDOWN:

		break;
		case WM_LBUTTONUP:

		break;
		case WM_MOUSEWHEEL:
			_fDist -= GET_WHEEL_DELTA_WPARAM( wParam ) / 100.0f;
		break;
		case WM_KEYDOWN:
		{
			switch ( wParam )
			{
				case VK_ESCAPE:
					PostMessage( hWnd, WM_DESTROY, 0, 0 );
				break;
			}
		}
		break;
		case WM_DESTROY:
			//FreeConsole();
			PostQuitMessage( 0 );
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
