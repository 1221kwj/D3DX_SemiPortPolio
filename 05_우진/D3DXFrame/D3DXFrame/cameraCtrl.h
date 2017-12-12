#pragma once
#include "singletonBase.h"

class cameraCtrl : public singletonBase <cameraCtrl>
{
private:
	D3DXVECTOR3 eyeVector;		//카메라의 위치
	D3DXVECTOR3 lookAtVector;	//카메라고 보는 지점
	D3DXVECTOR3 upVector;		//카메라의 업 벡터

	D3DXMATRIXA16 viewMatrix;
	D3DXMATRIXA16 projMatrix;

	float fDist;
	float fRotX;
	float fRotY;

	POINT	PrevMousePos;
	bool	isLButtonDown;

public:
	cameraCtrl();
	~cameraCtrl();

	void init( void );
	void update( D3DXVECTOR3* target );

	void cameraMove();
};

