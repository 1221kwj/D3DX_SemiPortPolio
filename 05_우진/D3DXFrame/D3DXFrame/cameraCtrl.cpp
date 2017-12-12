#include "stdafx.h"
#include "cameraCtrl.h"

cameraCtrl::cameraCtrl()
	: eyeVector		(D3DXVECTOR3(-10.0f, 5.0f, -10.0f))
	, lookAtVector	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, upVector		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
	, fDist			(5.0f)
	, fRotX			(0.0f)
	, fRotY			(0.0f)
	, isLButtonDown (false)
{
	D3DXMatrixIdentity( &viewMatrix );
	D3DXMatrixIdentity( &projMatrix );
}

cameraCtrl::~cameraCtrl() {}

void cameraCtrl::init( void )
{
	RECT rc;
	GetClientRect( _hWnd, &rc );

	D3DXMatrixLookAtLH(
		&viewMatrix,
		&eyeVector,
		&lookAtVector,
		&upVector );

	DEVICE->SetTransform( D3DTS_VIEW, &viewMatrix );

	D3DXMatrixPerspectiveFovLH( 
		&projMatrix,
		3.141592f / 4.0f,
		rc.right / ( float ) rc.bottom,
		1,
		1000
	);

	DEVICE->SetTransform( D3DTS_PROJECTION, &projMatrix );
}

void cameraCtrl::update( D3DXVECTOR3* target )
{
	cameraMove();

	eyeVector = D3DXVECTOR3( 0, 0, -fDist );

	D3DXMATRIX matRotaionX, matRotaionY, matRotaion;

	D3DXMatrixRotationX( &matRotaionX, fRotX );
	D3DXMatrixRotationY( &matRotaionY, fRotY );
	matRotaion = matRotaionX * matRotaionY;
	D3DXVec3TransformCoord( &eyeVector, &eyeVector, &matRotaion );

	matRotaion = matRotaionX * matRotaionY;
	D3DXVec3TransformCoord ( &eyeVector, &eyeVector, &( matRotaionX * matRotaionY ) );
	
	lookAtVector = *target;
	eyeVector = *target + eyeVector;

	D3DXMatrixLookAtLH( &viewMatrix, &eyeVector, &lookAtVector, &upVector );
	DEVICE->SetTransform( D3DTS_VIEW, &viewMatrix );
}

void cameraCtrl::cameraMove()
{
	if ( KEYMANAGER->isOnceKeyDown( VK_LBUTTON ) )
	{
		PrevMousePos.x = _ptMouse.x;
		PrevMousePos.y = _ptMouse.y;
		isLButtonDown = true;
	}
	else if ( KEYMANAGER->isOnceKeyUp( VK_LBUTTON ) )
	{
		isLButtonDown = false;
	}

	if ( isLButtonDown ) 
	{
		POINT curPos;
		curPos.x = _ptMouse.x;
		curPos.y = _ptMouse.y;

		fRotY += ( curPos.x - PrevMousePos.x ) / 100.0f;
		fRotX += ( curPos.y - PrevMousePos.y ) / 100.0f;

		if ( fRotX <= -D3DX_PI * 0.5f + D3DX_16F_EPSILON )
		{
			fRotX = -D3DX_PI * 0.5f + D3DX_16F_EPSILON;
		}

		if ( fRotX >= D3DX_PI * 0.5f + D3DX_16F_EPSILON )
		{
			fRotX = D3DX_PI * 0.5f + D3DX_16F_EPSILON;
		}

		PrevMousePos = curPos;
	}

	fDist = _fDist;
}
