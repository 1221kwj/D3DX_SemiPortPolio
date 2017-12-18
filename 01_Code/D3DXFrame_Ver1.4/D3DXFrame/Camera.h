#pragma once
#include "singletonBase.h"

#define MOUSEAPM 0.001f	// ¸¶¿ì½º ¹Î°¨µµ

class Camera : public singletonBase<Camera>
{
private:
	D3DXVECTOR3 eyeVector;
	D3DXVECTOR3 upVector;
	D3DXVECTOR3 orgLookAtVector;

	D3DXVECTOR3 viewVector;			// ÃÑ ½ò¶§ ÇÊ¿äÇÔ
	
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projMatrix;

	POINT PrevMousePos;
	POINT CurMousePos;

	float fLocalRotX;
	float fViewDistZ;
	float fViewDistX;
	float fHeight;

public:
	Camera();
	~Camera();

	void init();
	void update();

	void CameraSetting( D3DXVECTOR3 pEye, D3DXVECTOR3 pLookAt, D3DXVECTOR3 pUp );

	GETSET( float, fLocalRotY, LocalRotY );
	GETSET( D3DXVECTOR3, lookAtVector, LookAtVector );
};

