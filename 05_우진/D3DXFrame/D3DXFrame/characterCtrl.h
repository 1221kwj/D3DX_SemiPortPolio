#pragma once

class characterCtrl
{
public:
	characterCtrl();
	~characterCtrl();

	void update( Group* pGroup = nullptr );
	D3DXVECTOR3* GetPosition();
	D3DXMATRIXA16* GetWorldTM();

private:
	D3DXVECTOR3 positionVector;
	D3DXMATRIXA16 worldMatrix;
	float rotationY;
	float speed;
};

