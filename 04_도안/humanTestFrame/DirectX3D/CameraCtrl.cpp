#include "stdafx.h"
#include "CameraCtrl.h"


CameraCtrl::CameraCtrl ()
	: eyeVector ( 0, 0, -5 )
	, lookAtVector ( 0, 0, 0 )
	, upVector ( 0, 1, 0 )
	, fDistance ( 5 )
	, fRotX ( 0.0f )
	, fRotY ( 0.0f )
	, isLButtonDown ( false )
{
}

CameraCtrl::~CameraCtrl ()
{
}

void CameraCtrl::Init ()
{
	RECT rc;
	GetClientRect ( g_hWnd, &rc );

	D3DXMatrixLookAtLH ( &viewMatrix, &eyeVector, &lookAtVector, &upVector );
	D3DDevice->SetTransform ( D3DTS_VIEW, &viewMatrix );

	D3DXMatrixPerspectiveFovLH ( &projMatrix,
		3.141592f / 4.0f,
		rc.right / ( float ) rc.bottom,
		1,
		10000
	);

	D3DDevice->SetTransform ( D3DTS_PROJECTION, &projMatrix );
}

void CameraCtrl::Update ( D3DXVECTOR3 target )
{
	eyeVector = D3DXVECTOR3 ( 0, 0, -fDistance );

	D3DXMATRIX matRotaionX, matRotaionY, matRotaion;
	
	D3DXMatrixRotationX ( &matRotaionX, fRotX );	
	D3DXMatrixRotationY ( &matRotaionY, fRotY );	
	matRotaion =  matRotaionX * matRotaionY;
	D3DXVec3TransformCoord ( &eyeVector, &eyeVector, &matRotaion );	

	//matRotaion = matRotaionX * matRotaionY;
	//D3DXVec3TransformCoord ( &eyeVector, &eyeVector, &( matRotaionX * matRotaionY ) );


	lookAtVector = target;
	lookAtVector.z = -target.z;
	eyeVector = target + eyeVector;	
	
	D3DXMatrixLookAtLH(&viewMatrix, &eyeVector, &lookAtVector, &upVector );
	D3DDevice->SetTransform ( D3DTS_VIEW, &viewMatrix );

}


void CameraCtrl::WndProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

	switch ( msg )
	{
		case WM_LBUTTONDOWN:
			{
				PrevMousePos.x = LOWORD ( lParam );
				PrevMousePos.y = HIWORD ( lParam );
				isLButtonDown = true;
			}
			break;
		case WM_LBUTTONUP:
			{
				isLButtonDown = false;
			}
			break;
		case WM_MOUSEMOVE:
			{
				if ( isLButtonDown ) {
					POINT curPos;
					curPos.x = LOWORD ( lParam );
					curPos.y = HIWORD ( lParam );

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
			}
			break;
		case WM_MOUSEWHEEL:
			{
				fDistance -= GET_WHEEL_DELTA_WPARAM ( wParam ) / 100.0f;
			}
			break;
	}


}
















