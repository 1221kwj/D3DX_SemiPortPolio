#include "stdafx.h"
#include "characterCtrl.h"

characterCtrl::characterCtrl()
	: positionVector( 0, 0, 0 )
	, rotationY( 0 )
	, speed( 1.0f )
{
}

characterCtrl::~characterCtrl() {}

void characterCtrl::update( Group* pGroup )
{
	D3DXVECTOR3 tempPosition = positionVector;

	if ( KEYMANAGER->isStayKeyDown( 'A' ) )
	{
		rotationY -= 0.1f;
	}
	else if ( KEYMANAGER->isStayKeyDown( 'D' ) )
	{
		rotationY += 0.1f;
	}

	D3DXVECTOR3 vDirection( 0, 0, -1 );
	D3DXMATRIXA16 matRotationY;
	D3DXMatrixIdentity( &matRotationY );

	D3DXMatrixRotationY( &matRotationY, rotationY );
	D3DXVec3TransformNormal( &vDirection, &vDirection, &matRotationY );

	if ( KEYMANAGER->isStayKeyDown( 'W' ) )
	{
		tempPosition = tempPosition + vDirection * speed;
	}
	else if ( KEYMANAGER->isStayKeyDown( 'S' ) )
	{
		tempPosition = tempPosition - vDirection * speed;
	}

	if ( pGroup )
	{
		if ( pGroup->GetHeight( tempPosition.x, tempPosition.y, tempPosition.z ) )
		{
			positionVector = tempPosition;
		}
	}
	else
	{
		positionVector = tempPosition;
	}

	D3DXMATRIXA16 translationMatrix;
	D3DXMatrixTranslation( &translationMatrix, positionVector.x, positionVector.y, positionVector.z );

	worldMatrix = matRotationY * translationMatrix;
}

D3DXVECTOR3* characterCtrl::GetPosition()
{
	return &positionVector;
}

D3DXMATRIXA16* characterCtrl::GetWorldTM()
{
	return &worldMatrix;
}