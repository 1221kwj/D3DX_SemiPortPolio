#include "stdafx.h"
#include "MainGameManager.h"

//=============================
//   ## �߰��� ���� ��� ##
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

	//����׸� ���� ��������� ���̾� �������� �ٲ���
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
	//================= �ϼ��Ҷ����� �ǵ��� �ʴ°� ���� �� ===================
	//Ÿ�� ���� ���Ű
	if ( KEYMANAGER->isOnceKeyDown( '0' ) )
	{
		if (TIMEMANAGER->GetIsRender()) TIMEMANAGER->SetIsRender( false );
		else TIMEMANAGER->SetIsRender( true );
	}

	//�׸��� ���Ű
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
	//         ## �׸��� ���� ##
	//--------------------------------------
				
	SCENEMANAGER->render();				
	//--------------------------------------
	//         ## �׸��� ���� ##
	//--------------------------------------
	DEVICEMANAGER->renderEnd();		
}

