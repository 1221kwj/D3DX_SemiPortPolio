#include "stdafx.h"

#include "SkinnedMesh.h"

#include "testScene.h"

testScene:: testScene() {}
testScene::~testScene() {}

HRESULT testScene::init()
{
	control = new CharCtrl;
	control->init();

	test_Human = SKINNEDMESHMANAGER->GetSkinnedMesh( "»ç¶÷", "./zombie/human/", "human.x" );
	
	cameraSetting();

	return S_OK;
}

void testScene::cameraSetting()
{
	LPD3DXFRAME root = test_Human->GetRoot();

	D3DXMATRIX eyeWorldMat		= test_Human->GetCombineMatrix( "camera_tpp" );
	D3DXMATRIX targetWorldMat	= test_Human->GetCombineMatrix( "GunReferencePoint" );

	D3DXVECTOR3 eyePos = D3DXVECTOR3(
		eyeWorldMat._41,
		eyeWorldMat._42 + 1.5f,
		eyeWorldMat._43 + 5.5f );

	D3DXVECTOR3 targetPos = D3DXVECTOR3(
		targetWorldMat._41 - 1.2f,
		targetWorldMat._42 + 1.5f,
		targetWorldMat._43 );

	CAMERAMANAGER->CameraSetting( eyePos, targetPos, D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
}

void testScene::release()
{
}

void testScene::update()
{
	control->update();
	
	test_Human->SetPosition( control->GetPosition() );
	test_Human->SetAngle( CAMERAMANAGER->GetLocalRotY() );
	
	CAMERAMANAGER->update();

	if		( KEYMANAGER->isOnceKeyDown( 'O' ) ) ShowCursor( true );
	else if ( KEYMANAGER->isOnceKeyDown( 'P' ) ) ShowCursor( false );
}

void testScene::render()
{
	test_Human->UpdateAndRender();
}


