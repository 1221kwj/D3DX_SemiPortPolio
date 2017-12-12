#include "stdafx.h"
#include "iMap.h"

#include "CharacterCtrl.h"


CharacterCtrl::CharacterCtrl ()
	: positionVector ( 0, 0, 0 )
	, rotationY ( 0 )
	, speed ( 0.05f )
{

}


CharacterCtrl::~CharacterCtrl ()
{
}

void CharacterCtrl::Update ( iMap* pMap )
{

	D3DXVECTOR3 tempPosition = positionVector;


	if ( GetKeyState ( 'A' ) & 0x8000 ) {
		rotationY -= 0.1f;
	}

	if ( GetKeyState ( 'D' ) & 0x8000 ) {
		rotationY += 0.1f;
	}

	D3DXVECTOR3 vDirection ( 0, 0, 1 );
	D3DXMATRIXA16 matRotationY;
	D3DXMatrixIdentity ( &matRotationY );

	D3DXMatrixRotationY ( &matRotationY, rotationY );
	D3DXVec3TransformNormal ( &vDirection, &vDirection, &matRotationY );

	if ( GetKeyState ( 'S' ) & 0x8000 ) {
		tempPosition = tempPosition + vDirection * speed;
	}

	if ( GetKeyState ( 'W' ) & 0x8000 ) {
		tempPosition = tempPosition - vDirection * speed;
	}
	//°øÁßÀ¸·Î Á» ¶ç¾îÁÙ º¤ÅÍ
	D3DXVECTOR3 dd(0, 1, 0);
	
	if ( pMap )
	{
		if ( pMap->GetHeight ( tempPosition.x, tempPosition.y, tempPosition.z ) )
		{
			positionVector = tempPosition /*+ dd*/;
		}
	}
	else 
	{
		positionVector = tempPosition;
	}

	D3DXMATRIXA16 translationMatrix;
	D3DXMatrixTranslation ( &translationMatrix, positionVector.x, positionVector.y, positionVector.z );

	worldMatrix = matRotationY * translationMatrix;


}

D3DXVECTOR3 * CharacterCtrl::GetPosition ()
{
	return &positionVector;
}

D3DXMATRIXA16 * CharacterCtrl::GetWorldTM ()
{
	return &worldMatrix;
}

