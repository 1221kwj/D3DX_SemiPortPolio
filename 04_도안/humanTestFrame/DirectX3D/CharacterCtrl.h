#pragma once

class iMap;

class CharacterCtrl
{
public:
	CharacterCtrl ();
	~CharacterCtrl ();

	void Update ( iMap* pMap = nullptr );
	D3DXVECTOR3* GetPosition ();
	D3DXMATRIXA16* GetWorldTM ();
	//D3DXMATRIX* GetWorld();

private:
	D3DXVECTOR3 positionVector;
	D3DXMATRIXA16 worldMatrix;
	float rotationY;
	float speed;


};

