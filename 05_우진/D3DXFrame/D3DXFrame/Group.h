#pragma once

#include "MaterialTexture.h"

class Group : public Object
{
public:
	Group();
	~Group();

	void release();
	void render();
	bool GetHeight( IN const float& x, OUT float& y, IN const float& z );
	
	void setScale		( D3DXVECTOR3 pScale );
	void SetRotationY	( float pRotY );
	void setPosition	( D3DXVECTOR3 pPos );

protected:
	D3DXMATRIXA16	worldTM;
	D3DXMATRIXA16	scaleMat;
	D3DXMATRIXA16	rotYMat;
	D3DXMATRIXA16	transMat;
	D3DXVECTOR3		position;
	
	GETSET_ADD_REF(MaterialTexture*, materialTexture, MaterialTexture);
	GETSET_REF( vector<VertexPNT>, PNTVerts, PNTVerts );
	GETSET_REF( vector<D3DXVECTOR3>, mapVertexList, MapVertexList );
	GETSET( int, tileCount, TileCount );
};

