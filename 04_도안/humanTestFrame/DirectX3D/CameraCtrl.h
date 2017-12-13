#pragma once
class CameraCtrl
{
public:
	CameraCtrl ();
	~CameraCtrl ();

	void Init ();
	void Update ( D3DXVECTOR3 target );
	void WndProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

private:
	D3DXVECTOR3 eyeVector;
	D3DXVECTOR3 lookAtVector;
	D3DXVECTOR3 upVector;

	D3DXMATRIX viewMatrix;
	D3DXMATRIX projMatrix;

	float fDistance;
	float fRotX;
	float fRotY;


	POINT PrevMousePos;
	bool  isLButtonDown;

};

