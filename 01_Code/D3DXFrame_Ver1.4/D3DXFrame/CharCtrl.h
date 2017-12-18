#pragma once

#define MOVEAPM	1.2f

enum CHAR_STATE
{
	CHAR_WALK, //¿¹½Ã
	CHAR_NONE
};

class CharCtrl
{
private:
	float angle;

public:
	CharCtrl();
	~CharCtrl();

	void init();
	void update( SkinnedMesh* pMesh = nullptr );

	GETSET( D3DXVECTOR3, position, Position );
	GETSET( D3DXMATRIX, matWorld, MatWorld );
};

