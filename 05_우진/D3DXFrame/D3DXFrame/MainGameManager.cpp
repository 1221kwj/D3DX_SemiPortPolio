#include "stdafx.h"
#include "MainGameManager.h"

//=============================
//   ## 추가할 씬의 헤더 ##
//=============================
#include "testScene.h"

MainGameManager:: MainGameManager()	{}
MainGameManager::~MainGameManager() {}

HRESULT MainGameManager::init( void )
{
	gameNode::init( true );

	SCENEMANAGER->addScene( "test", new testScene );
	SCENEMANAGER->changeScene( "test" );

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
}

void MainGameManager::render( void )
{
	DEVICEMANAGER->renderBegin();

	if ( KEYMANAGER->isOnceKeyDown( '0' ) )
	{
		if (TIMEMANAGER->GetIsRender()) TIMEMANAGER->SetIsRender( false );
		else TIMEMANAGER->SetIsRender( true );
	}

	if ( KEYMANAGER->isOnceKeyDown( '9' ) )
	{
		if ( GRIDMANAGER->GetIsRender() ) GRIDMANAGER->SetIsRender( false );
		else GRIDMANAGER->SetIsRender( true );
	}

	TIMEMANAGER->render();
	GRIDMANAGER->render();
	//--------------------------------------
	//         ## 그리기 시작 ##
	//--------------------------------------
				
	SCENEMANAGER->render();				
	//--------------------------------------
	//         ## 그리기 시작 ##
	//--------------------------------------
	DEVICEMANAGER->renderEnd();		
}
