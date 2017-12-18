#include "stdafx.h"
#include "MainGameManager.h"

//=============================
//   ## 추가할 씬의 헤더 ##
//=============================
#include "testScene.h"
#include "IntroLoadingScene.h"

MainGameManager:: MainGameManager()	{}
MainGameManager::~MainGameManager() {}

HRESULT MainGameManager::init( void )
{
	gameNode::init( true );

	WireFrameOn = false;

	SCENEMANAGER->addScene( "test", new testScene );
	SCENEMANAGER->addScene( "load", new IntroLoadingScene);
	SCENEMANAGER->changeScene( "load" );

	DEVICE->SetRenderState( D3DRS_NORMALIZENORMALS, true );
	DEVICE->SetRenderState( D3DRS_LIGHTING, false );

	return S_OK;
}

void MainGameManager::release( void )
{
	gameNode::release();

}

void MainGameManager::update( void )
{
	gameNode::update();

	SCENEMANAGER->update();

	//디버그를 위한 렌더모양을 와이어 프레임을 바꿔줌
	if ( KEYMANAGER->isOnceKeyDown( VK_SPACE ) )
	{
		if ( !WireFrameOn )
		{
			WireFrameOn = true;
		}
		else
		{
			WireFrameOn = false;
		}
	}
}

void MainGameManager::render( void )
{
	DEVICEMANAGER->renderBegin();
	//================= 완성할때까지 건들지 않는게 좋을 듯 ===================
	//타임 라인 토글키
	if ( KEYMANAGER->isOnceKeyDown( '0' ) )
	{
		if (TIMEMANAGER->GetIsRender()) TIMEMANAGER->SetIsRender( false );
		else TIMEMANAGER->SetIsRender( true );
	}

	//그리드 토글키
	if ( KEYMANAGER->isOnceKeyDown( '9' ) )
	{
		if ( GRIDMANAGER->GetIsRender() ) GRIDMANAGER->SetIsRender( false );
		else GRIDMANAGER->SetIsRender( true );
	}
	
	if ( WireFrameOn )
	{
		DEVICE->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	}
	else
	{
		DEVICE->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	}

	TIMEMANAGER->render();
	GRIDMANAGER->render();
	//=========================================================================
	
	//--------------------------------------
	//         ## 그리기 시작 ##
	//--------------------------------------
				
	SCENEMANAGER->render();				
	//--------------------------------------
	//         ## 그리기 시작 ##
	//--------------------------------------
	DEVICEMANAGER->renderEnd();		
}

