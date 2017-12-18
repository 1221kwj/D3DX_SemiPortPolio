#include "stdafx.h"

#include "SkinnedMesh.h"

#include "testScene.h"

testScene:: testScene() {}
testScene::~testScene() {}

HRESULT testScene::init()
{
	control = new CharCtrl;
	control->init();
	
	test_Human = SKINNEDMESHMANAGER->GetSkinnedMesh( "사람" );
	test_Human->SetAnimationIndex(26);
	
	m4 = SKINNEDMESHMANAGER->GetSkinnedMesh("m4");
	akm = SKINNEDMESHMANAGER->GetSkinnedMesh("akm");
	pan = SKINNEDMESHMANAGER->GetSkinnedMesh("pan");

	zombie1 = new enemy;
	zombie1->init("좀비", D3DXVECTOR3(40, 0, 20));

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
	zombie1->update();
	test_Human->SetPosition( control->GetPosition() );
	test_Human->SetAngle( CAMERAMANAGER->GetLocalRotY() );
	
	CAMERAMANAGER->update();
	
	if		( KEYMANAGER->isOnceKeyDown( 'O' ) ) ShowCursor( true );
	else if ( KEYMANAGER->isOnceKeyDown( 'P' ) ) ShowCursor( false );

	charKeySet();
	//장비
	LPD3DXFRAME first = D3DXFrameFind(test_Human->GetRoot(), "slot_primary");
	LPD3DXFRAME ak = D3DXFrameFind(akm->GetRoot(), "Gun_root");
	first->pFrameFirstChild = ak;

	LPD3DXFRAME second = D3DXFrameFind(test_Human->GetRoot(), "ik_hand_gun");
	LPD3DXFRAME h4 = D3DXFrameFind(m4->GetRoot(), "Gun_root");
	second->pFrameFirstChild = h4;


	LPD3DXFRAME melee = D3DXFrameFind(pan->GetRoot(), "ROOT");
	LPD3DXFRAME meleeSlot = D3DXFrameFind(test_Human->GetRoot(), "slot_melee");
	melee->pFrameFirstChild = melee;

	m4Setting();
}

void testScene::render()
{
	test_Human->UpdateAndRender();
	zombie1->render();
	SKINNEDMESHMANAGER->GetNoBoneMesh("터널")->Render();
}

void testScene::charKeySet()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		test_Human->SetAnimationIndex(2, true);
	}
	else if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		test_Human->SetAnimationIndex(26, true);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		test_Human->SetAnimationIndex(11, true);
	}
	if (KEYMANAGER->isOnceKeyDown('S')) test_Human->SetAnimationIndex(20 - 5, true);
	if (KEYMANAGER->isOnceKeyDown('W')) test_Human->SetAnimationIndex(21 - 5, true);
	if (KEYMANAGER->isOnceKeyDown('A')) test_Human->SetAnimationIndex(19 - 5, true);
	if (KEYMANAGER->isOnceKeyDown('D')) test_Human->SetAnimationIndex(18 - 5, true);
	if (KEYMANAGER->isOnceKeyUp('W') || KEYMANAGER->isOnceKeyUp('S') || KEYMANAGER->isOnceKeyUp('A') || KEYMANAGER->isOnceKeyUp('D'))
		test_Human->SetAnimationIndex(31 - 5, true);
}

void testScene::m4Setting()
{
	//보정기
	if (m4Set.isCompen)
	{
		LPD3DXFRAME compenParts = D3DXFrameFind(m4->GetRoot(), "Gun_compenParts");
		SkinnedMesh* mesh = SKINNEDMESHMANAGER->GetSkinnedMesh("보정기");
		LPD3DXFRAME compenFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");
		compenParts->pFrameFirstChild = compenFrame;
	}
	else
	{

	}

	//손잡이
	if (m4Set.isGrip)
	{
		LPD3DXFRAME gripParts = D3DXFrameFind(m4->GetRoot(), "Gun_gripParts");
		SkinnedMesh* mesh = SKINNEDMESHMANAGER->GetSkinnedMesh("앵글손잡이");
		LPD3DXFRAME gripFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");
		gripParts->pFrameFirstChild = gripFrame;
	}
	else
	{

	}

	//개머리판
	if (m4Set.isStock)
	{
		LPD3DXFRAME stockParts = D3DXFrameFind(m4->GetRoot(), "Gun_stockParts");
		SkinnedMesh* mesh = SKINNEDMESHMANAGER->GetSkinnedMesh("개머리판");
		LPD3DXFRAME stockFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");
		stockParts->pFrameFirstChild = stockFrame;
	}
	else
	{
		LPD3DXFRAME stockParts = D3DXFrameFind(m4->GetRoot(), "Gun_stockParts");
		SkinnedMesh* mesh = SKINNEDMESHMANAGER->GetSkinnedMesh("기본개머리판");
		LPD3DXFRAME stockFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");
		stockParts->pFrameFirstChild = stockFrame;
	}

	//탄창
	if (m4Set.isMag)
	{
		LPD3DXFRAME magParts = D3DXFrameFind(m4->GetRoot(), "Gun_magazineParts");
		SkinnedMesh* mesh = SKINNEDMESHMANAGER->GetSkinnedMesh("대탄");
		LPD3DXFRAME magFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");
		magParts->pFrameFirstChild = magFrame;
	}
	else
	{
		LPD3DXFRAME magParts = D3DXFrameFind(m4->GetRoot(), "Gun_magazineParts");
		SkinnedMesh* mesh = SKINNEDMESHMANAGER->GetSkinnedMesh("기본탄");
		LPD3DXFRAME magFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");
		magParts->pFrameFirstChild = magFrame;
	}

	//가늠좌
	if (m4Set.isSight)
	{
		LPD3DXFRAME sightParts = D3DXFrameFind(m4->GetRoot(), "Gun_sightParts");
		SkinnedMesh* mesh = SKINNEDMESHMANAGER->GetSkinnedMesh("4배율");
		LPD3DXFRAME sightFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");
		sightParts->pFrameFirstChild = sightFrame;
	}
	else
	{
		LPD3DXFRAME sightParts = D3DXFrameFind(m4->GetRoot(), "Gun_sightParts");
		SkinnedMesh* mesh = SKINNEDMESHMANAGER->GetSkinnedMesh("가늠좌");
		LPD3DXFRAME sightFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");
		sightParts->pFrameFirstChild = sightFrame;
	}
}
