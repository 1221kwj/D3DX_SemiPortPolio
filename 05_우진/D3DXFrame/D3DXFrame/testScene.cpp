#include "stdafx.h"
#include "skyBox.h"
#include "testScene.h"
#include <algorithm>

testScene:: testScene() {}
testScene::~testScene() {}

HRESULT testScene::init()
{
	skybox = new skyBox;
	skybox->init();
	controller = new characterCtrl;

	SKINNEDMESHMANAGER->GetSkinnedMesh( "사람", "./zombie/human/", "human.x" );
	SKINNEDMESHMANAGER->GetSkinnedMesh( "사람" )->SetRandomTrackPosition();
	OBJLOADERMANAGER->addOBJFile( "map_02", "./BG_Resource/03_Map/map_02/", "map_02.obj" );
	OBJLOADERMANAGER->addOBJFile( "map_03", "./BG_Resource/03_Map/map_03/", "map_03.obj" );
	
	mapGroupList_1 = OBJLOADERMANAGER->GetGroup( "map_02" );
	mapGroupList_2 = OBJLOADERMANAGER->GetGroup( "map_03" );
	/*D3DXMATRIXA16 scaleTM;
	D3DXMatrixIdentity( &scaleTM );
	D3DXMatrixScaling( &scaleTM, 4.0f, 1.0f, 1.0f );*/

	D3DXMATRIXA16 transTM;
	D3DXMatrixIdentity(&transTM );
	D3DXMatrixTranslation( &transTM, 0, 0, -240 );

	for each ( auto iter in mapGroupList_1 )
	{
		int vSize = iter->GetPNTVerts().size();
		/*int mSize = iter->GetMapVertexList().size();*/

		for ( int i = 0; i < vSize; i++ )
		{
			iter->GetPNTVerts()[ i ].p /= 64;
			//D3DXVec3TransformCoord( &iter->GetPNTVerts()[ i ].p, &iter->GetPNTVerts()[ i ].p, &scaleTM );
		}

		//for ( int i = 0; i < mSize; i++ )
		//{
		//	iter->GetMapVertexList()[ i ] /= 64;
		//}
	}

	for each ( auto iter in mapGroupList_2 )
	{
		int vSize = iter->GetPNTVerts().size();

		for ( int i = 0; i < vSize; i++ )
		{
			iter->GetPNTVerts()[ i ].p /= 64;
			D3DXVec3TransformCoord( &iter->GetPNTVerts()[ i ].p, &iter->GetPNTVerts()[ i ].p, &transTM );
		}
	}
	return S_OK;
}

void testScene::release()
{
	SAFE_DELETE( controller );
	SAFE_DELETE( skybox );
}

void testScene::update()
{

	if ( ( *controller->GetPosition() ).z >= -239 )
	{
		for each ( auto iter in mapGroupList_1 )
		{
			controller->update( iter );
		}
	}
	else if (( *controller->GetPosition() ).z <= -239 )
	{
		for each ( auto iter in mapGroupList_2 )
		{
			controller->update( iter );
		}
	}
	
	//controller->update();
	CAMERAMANAGER->update(controller->GetPosition());


	SKINNEDMESHMANAGER->GetSkinnedMesh( "사람" )->SetWorldTM( *controller->GetWorldTM() );
}

void testScene::render()
{
	char str[ 256 ];
	string frameRate;
	RECT RC_COORD;
	SetRect( &RC_COORD, WINSIZEX / 2, 0, WINSIZEX / 2 + 100, 20 );

	D3DXVECTOR3 position = *controller->GetPosition();
	LPD3DXFONT font = FONTMANAGER->GetFont( fontManager::NORMAL );

	sprintf_s( str, "x : %5.2f, y : %5.2f,  z : %5.2f", position.x, position.y, position.z );
	font->DrawTextA( nullptr, str, 256, &RC_COORD, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB( 255, 0, 0 ) );


	//DEVICE->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	D3DXMATRIXA16 worldTM;
	D3DXMatrixIdentity( &worldTM );
	DEVICE->SetTransform( D3DTS_WORLD, &worldTM );
	
	skybox->render();

	for each ( auto iter in mapGroupList_1 ) iter->render();
	for each ( auto iter in mapGroupList_2 ) iter->render();

	SKINNEDMESHMANAGER->GetSkinnedMesh( "사람" )->UpdateAndRender();
}
