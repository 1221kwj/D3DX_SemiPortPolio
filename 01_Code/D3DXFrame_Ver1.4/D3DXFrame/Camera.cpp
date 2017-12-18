#include "stdafx.h"
#include "Camera.h"

Camera:: Camera() {}
Camera::~Camera() {}

void Camera::init()
{
	eyeVector		= D3DXVECTOR3( 0.0f, 0.0f, -5.0f );
	upVector		= D3DXVECTOR3( 0.0f, 1.0f,  0.0f );
	lookAtVector	= D3DXVECTOR3( 0.0f, 0.0f,  0.0f );
	orgLookAtVector = lookAtVector;

	D3DXVec3Normalize( &viewVector, &( lookAtVector - eyeVector ) );

	D3DXMatrixIdentity( &viewMatrix );
	D3DXMatrixIdentity( &projMatrix );

	PrevMousePos	= { WINSIZEX / 2, WINSIZEY / 2 };
	CurMousePos		= { WINSIZEX / 2, WINSIZEY / 2 };

	fLocalRotX	= 0.0f;
	fLocalRotY	= 0.0f;

	fViewDistZ	= fabs( lookAtVector.z - eyeVector.z );
	fViewDistX	= fabs( lookAtVector.x - eyeVector.x );
	fHeight		= fabs( lookAtVector.y - eyeVector.y );

	D3DXMatrixLookAtLH(
		&viewMatrix,
		&eyeVector,
		&lookAtVector,
		&upVector
	);
	DEVICE->SetTransform( D3DTS_VIEW, &viewMatrix );

	D3DXMatrixPerspectiveFovLH(
		&projMatrix,
		D3DX_PI * 0.25,
		WINSIZEX / ( float )WINSIZEY,
		1.0f,
		10000.0f
	);
	DEVICE->SetTransform( D3DTS_PROJECTION, &projMatrix );
}

void Camera::update()
{
	eyeVector = D3DXVECTOR3( 0.0f, fViewDistX, fViewDistZ );

	D3DXMATRIX matLocalRotX, matLocalRotY, matLocalRot;

	D3DXMatrixRotationX( &matLocalRotX, fLocalRotX );
	D3DXMatrixRotationY( &matLocalRotY, fLocalRotY );

	matLocalRot = matLocalRotX * matLocalRotY;
	D3DXVec3TransformCoord( &eyeVector, &eyeVector, &matLocalRot );

	D3DXVECTOR3 curLookAt = lookAtVector;
	D3DXVECTOR3 rotLookAtVector = orgLookAtVector;
	
	D3DXVec3TransformCoord( &rotLookAtVector, &orgLookAtVector, &matLocalRotY );
	curLookAt = rotLookAtVector + lookAtVector;
	eyeVector = curLookAt + eyeVector;

	D3DXVec3Normalize( &viewVector, &( curLookAt - eyeVector ) );

	D3DXMatrixLookAtLH(
		&viewMatrix,
		&eyeVector,
		&curLookAt,
		&upVector
	);

	DEVICE->SetTransform( D3DTS_VIEW, &viewMatrix );
}

void Camera::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_MOUSEMOVE:
		GetCursorPos( &CurMousePos );

		fLocalRotX += ( CurMousePos.y - PrevMousePos.y) * MOUSEAPM;
		fLocalRotY += ( CurMousePos.x - PrevMousePos.x) * MOUSEAPM;

		if ( fLocalRotX <= -D3DX_PI * 0.4 - D3DX_16F_EPSILON )
		{
			fLocalRotX = -D3DX_PI * 0.4 - D3DX_16F_EPSILON;
		}
		if ( fLocalRotX >= D3DX_PI * 0.4 + D3DX_16F_EPSILON )
		{
			fLocalRotX = D3DX_PI * 0.4 + D3DX_16F_EPSILON;
		}

		SetCursorPos( PrevMousePos.x, PrevMousePos.y );

		break;
	}
}

void Camera::CameraSetting( D3DXVECTOR3 pEye, D3DXVECTOR3 pLookAt, D3DXVECTOR3 pUp )
{
	eyeVector		= pEye;
	lookAtVector	= pLookAt;
	upVector		= pUp;
	orgLookAtVector = lookAtVector;

	D3DXVec3Normalize( &viewVector, &( lookAtVector - eyeVector ) );

	fViewDistZ	= fabs( lookAtVector.z - eyeVector.z );
	fViewDistX	= fabs( lookAtVector.x - eyeVector.x );
	fHeight		= fabs( lookAtVector.y - eyeVector.y );
}