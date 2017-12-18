#include "stdafx.h"
#include "CharCtrl.h"

// X 축이 좌우
// Z 축이 앞뒤

CharCtrl::CharCtrl()
{

}

CharCtrl::~CharCtrl()
{
}

void CharCtrl::init()
{
	position	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	angle		= 0.0f;

	D3DXMatrixIdentity( &matWorld );
}

void CharCtrl::update( SkinnedMesh* pMesh )
{
	angle = CAMERAMANAGER->GetLocalRotY();

	D3DXMATRIX matRot;
	D3DXVECTOR3 xDirection( -1.0f, 0.0f,  0.0f );
	D3DXVECTOR3 zDirection(  0.0f, 0.0f, -1.0f );
	D3DXMatrixRotationAxis( &matRot, &D3DXVECTOR3(0, 1, 0), angle );
	
	D3DXVec3TransformNormal( &xDirection, &xDirection, &matRot );
	D3DXVec3TransformNormal( &zDirection, &zDirection, &matRot );

	if ( KEYMANAGER->isStayKeyDown('A' ))
	{
		position = position - xDirection * 0.5f;
	}
	else if ( KEYMANAGER->isStayKeyDown( 'D' ) )
	{
		position = position + xDirection * 0.5f;
	}
	
	if ( KEYMANAGER->isStayKeyDown( 'W' ) )
	{
		position = position + zDirection * 0.5f;
	}
	else if ( KEYMANAGER->isStayKeyDown( 'S' ) )
	{
		position = position - zDirection * 0.5f;
	}

	D3DXMATRIX pMat;
	D3DXMatrixTranslation( &pMat, position.x, position.y, position.z );

	CAMERAMANAGER->SetLookAtVector( position );

	matWorld = matRot * pMat;
}