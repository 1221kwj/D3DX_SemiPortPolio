#include "stdafx.h"

#include "CubeCtrl.h"
#include "OBJLoader.h"
#include "SkyBox.h"

#include "Group.h"
#include "iMap.h"
#include "OBJMap.h"
#include "HeightMap.h"

#include "MainGameManager.h"

MainGameManager::MainGameManager ()
{
}

MainGameManager::~MainGameManager ()
{
}

void MainGameManager::Init ()
{
	
	cameraCtrl.Init ();
	
	gridCtrl.Init();

	D3DXMATRIXA16 worldMat;
	//D3DXMatrixIdentity(&worldMat);
	//D3DDevice->SetTransform(D3DTS_WORLD, &worldMat);
	D3DXMATRIXA16 scaleMat, rotateMat;
	//D3DXMatrixScaling(&scaleMat, 0.01f, 0.01f, 0.01f);
	//D3DXMatrixRotationX(&rotateMat, -D3DX_PI / 2.0f);
	//worldMat = scaleMat * rotateMat;
	
	OBJLoader* _ob = new OBJLoader;
	//_ob->Load("./obj/Map.obj", &worldMat, groupList);
	_ob->Load("./obj/Map_surface.obj", &worldMat, groupList2);



	mesh = _ob->LoadMesh("./obj/Map.obj", &worldMat, mList);

	woman = new ASECharacter;
	woman->Load("./woman/woman_01_all.ASE");
	
	map = new HeightMap;
	map->Load("./HeightMapData/HeightMap.raw", &worldMat);
	map->SetTexture(g_pTextureManager->GetTexture("./HeightMapData/terrain.jpg"));
	
	for (int x = -10; x < 11; x++)
	{
		for (int z = -10; z < 11; z++)
		{
			
		}
	}
	/*
	SkinnedMesh* skin = new SkinnedMesh("zombie/", "human.x");
	skin->SetPosition(D3DXVECTOR3(0, 0, 0));
	skin->SetAnimationIndex(0);
	//skin->SetRandomTrackPosition();
	skinList.push_back(skin);
	*/
	zombie = new SkinnedMesh("zombie/human/", "human.x");
	zombie->SetPosition(D3DXVECTOR3(0, 0, 0));
	zombie->SetAnimationIndex(9);

	m4 = new SkinnedMesh("m4/", "m4_base.x");
	m4->SetAnimationIndex(0);

	akm = new SkinnedMesh("akm/", "akm_base.x");
	akm->SetAnimationIndex(0);

	pan = new SkinnedMesh("weapon/", "pan.x");
	pan->SetAnimationIndex(0);


	coat = new SkinnedMesh("coat/", "coat.x");

	D3DXMATRIXA16 id;
	D3DXMatrixIdentity(&id);


	Culling = new FrustumCulling;

	//ºû
	SetLight ();

	D3DDevice->SetRenderState ( D3DRS_NORMALIZENORMALS, true );
	D3DDevice->SetRenderState ( D3DRS_LIGHTING, true );
	//D3DDevice->SetRenderState ( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	skyBox.Init();

	//m4Set
	m4Set.isCompen = m4Set.isGrip = m4Set.isMag = m4Set.isSight = m4Set.isStock = false;
	
}

void MainGameManager::Update ()
{
	g_pTimeManager->Update();
	zombie->SetPosition(D3DXVECTOR3(posX, posY, posZ));
	D3DXMATRIX* localTM;
	//D3DXMatrixIdentity(&localTM);

	//localTM = characterCtrl.GetWorldTM();
	//characterCtrl.Update(map);
	
	if (GetKeyState('A') & 0x8000)
	{
		indexNum++;
		if (indexNum > 8) indexNum = 0;
		for each (auto a in skinList)
		{
			a->SetAnimationIndex(indexNum);
		}
	}
	
	LPD3DXFRAME camera = D3DXFrameFind(zombie->GetRoot(), "camera_tpp");
	D3DXMATRIXA16 cameraTM = camera->TransformationMatrix;
	D3DXVECTOR3 cameraPos = D3DXVECTOR3(cameraTM._41, cameraTM._42, cameraTM._43);
	Culling->Update();
	cameraCtrl.Update ( *characterCtrl.GetPosition() );
	//cameraCtrl.Update(cameraPos);

	m4Setting();

	if (zombie->GetAnicontroller()->GetNumAnimationSets() == 1)
	{
		LPD3DXFRAME first = D3DXFrameFind(zombie->GetRoot(), "slot_primary");
		LPD3DXFRAME ak = D3DXFrameFind(akm->GetRoot(), "Gun_root");
		g_pSkinnedMeshManager->addChild(first, ak);

		LPD3DXFRAME second = D3DXFrameFind(zombie->GetRoot(), "slot_secondary");
		LPD3DXFRAME h4 = D3DXFrameFind(m4->GetRoot(), "Gun_root");
		g_pSkinnedMeshManager->addChild(second, h4);
	}

	else
	{
		LPD3DXFRAME hand = D3DXFrameFind(zombie->GetRoot(), "ik_hand_gun");
		LPD3DXFRAME gun = D3DXFrameFind(m4->GetRoot(), "Gun_root");
		g_pSkinnedMeshManager->addChild(hand, gun);
	}
	

	LPD3DXFRAME melee = D3DXFrameFind(pan->GetRoot(), "ROOT");
	LPD3DXFRAME meleeSlot = D3DXFrameFind(zombie->GetRoot(), "slot_melee");
	g_pSkinnedMeshManager->addChild(meleeSlot, melee);

	wearing();

}

void MainGameManager::Render (  )
{
	skyBox.Render();
	
	//m4->UpdateAndRender();
	
	//Culling->IsFrustum(zombie->GetBoundingSphere());
	
	//if(zombie->GetBoundingSphere()->isInFrustum) 
	
	zombie->UpdateAndRender();
	
	//coat->UpdateAndRender();

	gridCtrl.Render();
}

void MainGameManager::WndProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	cameraCtrl.WndProc ( hWnd, msg, wParam, lParam );
	switch (msg)
	{
		//ÆÄ¶û °ü·Ã Å¬¸¯
		case WM_LBUTTONDOWN:
		{
			

		}
		break;
		//»¡°­ °ü·Ã Å¬¸¯
		case WM_RBUTTONDOWN:
		{
			
		}
		break;
		case WM_KEYDOWN:
		{
			if (wParam == VK_SPACE)
			{
				zombie->SetAnimationIndex(0);
			}
			if (wParam == 'S')
			{
				posZ += 0.5f;
				zombie->SetAnimationIndex(3);
				
			}
			if (wParam == 'W')
			{
				posZ -= 0.5f;
				zombie->SetAnimationIndex(4);
				
			}
			if (wParam == 'A')
			{
				posX += 0.5f;
				zombie->SetAnimationIndex(2);
				
			}
			if (wParam == 'D')
			{
				posX -= 0.5f;
				zombie->SetAnimationIndex(1);
				
			}
			if (wParam == 'Z')
			{
				m4Set.isCompen = true;
			}
			if (wParam == 'X')
			{
				m4Set.isStock = true;
			}
			if (wParam == 'C')
			{
				m4Set.isMag = true;
			}
			if (wParam == 'V')
			{
				m4Set.isSight = true;
			}
			if (wParam == 'B')
			{
				m4Set.isGrip = true;
			}
		}
		break;
		case WM_KEYUP:
		{
			if (wParam == 'W' || wParam == 'S' || wParam == 'A' || wParam == 'D' || wParam == VK_SPACE)
			{
				zombie->SetAnimationIndex(5);
			}
		}
		break;
		
	}
}

void MainGameManager::Destroy()
{
	
	SAFE_RELEASE ( cubeCtrl );
	SAFE_RELEASE( woman );

	for each( auto p in  groupList ) {
		SAFE_RELEASE ( p );
	}
	
	for each( auto p in  groupList2 ) {
		SAFE_RELEASE ( p );
	}

}

void MainGameManager::SetLight ()
{
	D3DXVECTOR3 direction ( rand () % 100, -rand () % 100, rand () % 100 );
	D3DXVec3Normalize ( &direction, &direction );

	D3DLIGHT9 light;
	ZeroMemory ( &light, sizeof ( D3DLIGHT9 ) );

	light.Ambient = light.Diffuse = light.Specular = D3DXCOLOR ( 0.8f, 0.8f, 0.8f, 1.0f );
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = direction;

	D3DDevice->SetLight ( 0, &light );
	D3DDevice->LightEnable ( 0, true );

}

void MainGameManager::m4Setting()
{

	
	//º¸Á¤±â
	if (m4Set.isCompen)
	{
		LPD3DXFRAME compenParts = D3DXFrameFind(m4->GetRoot(), "Gun_compenParts");
		SkinnedMesh* mesh = new SkinnedMesh("m4/", "m4_compen.x");
		LPD3DXFRAME compenFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");

		g_pSkinnedMeshManager->addChild(compenParts, compenFrame);
	}
	else
	{

	}
	
	//¼ÕÀâÀÌ
	if (m4Set.isGrip)
	{
		LPD3DXFRAME gripParts = D3DXFrameFind(m4->GetRoot(), "Gun_gripParts");
		SkinnedMesh* mesh = new SkinnedMesh("m4/", "m4_grip.x");
		LPD3DXFRAME gripFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");

		g_pSkinnedMeshManager->addChild(gripParts, gripFrame);
	}
	else
	{

	}
	
	//°³¸Ó¸®ÆÇ
	if (m4Set.isStock)
	{
		LPD3DXFRAME stockParts = D3DXFrameFind(m4->GetRoot(), "Gun_stockParts");
		SkinnedMesh* mesh = new SkinnedMesh("m4/", "m4_stock.x");
		LPD3DXFRAME stockFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");

		g_pSkinnedMeshManager->addChild(stockParts, stockFrame);
	}
	else
	{
		LPD3DXFRAME stockParts = D3DXFrameFind(m4->GetRoot(), "Gun_stockParts");
		SkinnedMesh* mesh = new SkinnedMesh("m4/", "m4_base_stock.x");
		LPD3DXFRAME stockFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");

		g_pSkinnedMeshManager->addChild(stockParts, stockFrame);
	}
	
	//ÅºÃ¢
	if (m4Set.isMag)
	{
		LPD3DXFRAME magParts = D3DXFrameFind(m4->GetRoot(), "Gun_magazineParts");
		SkinnedMesh* mesh = new SkinnedMesh("m4/", "m4_lemag.x");
		LPD3DXFRAME magFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");

		g_pSkinnedMeshManager->addChild(magParts, magFrame);
	}
	else
	{
		LPD3DXFRAME magParts = D3DXFrameFind(m4->GetRoot(), "Gun_magazineParts");
		SkinnedMesh* mesh = new SkinnedMesh("m4/", "m4_base_mag.x");
		LPD3DXFRAME magFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");

		g_pSkinnedMeshManager->addChild(magParts, magFrame);
	}
	
	//°¡´ÆÁÂ
	if (m4Set.isSight)
	{
		LPD3DXFRAME sightParts = D3DXFrameFind(m4->GetRoot(), "Gun_sightParts");
		SkinnedMesh* mesh = new SkinnedMesh("m4/", "m4_acog.x");
		LPD3DXFRAME sightFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");

		g_pSkinnedMeshManager->addChild(sightParts, sightFrame);
	}
	else
	{
		LPD3DXFRAME sightParts = D3DXFrameFind(m4->GetRoot(), "Gun_sightParts");
		SkinnedMesh* mesh = new SkinnedMesh("m4/", "m4_base_sight.x");
		LPD3DXFRAME sightFrame = D3DXFrameFind(mesh->GetRoot(), "Gun_root");

		g_pSkinnedMeshManager->addChild(sightParts, sightFrame);
	}
	
}

void MainGameManager::wearing()
{
	
	LPD3DXFRAME coatFrame = D3DXFrameFind(coat->GetRoot(), "root");
	LPD3DXFRAME playerFrame = D3DXFrameFind(zombie->GetRoot(), "root");

	g_pSkinnedMeshManager->change(playerFrame, coatFrame);
}





