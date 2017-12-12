#pragma once
#include "singletonBase.h"

class cameraCtrl : public singletonBase <cameraCtrl>
{
private:
	D3DXVECTOR3 eyeVector;		//ī�޶��� ��ġ
	D3DXVECTOR3 lookAtVector;	//ī�޶�� ���� ����
	D3DXVECTOR3 upVector;		//ī�޶��� �� ����

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

