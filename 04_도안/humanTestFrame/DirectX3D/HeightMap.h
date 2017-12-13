#pragma once
#include "iMap.h"

#define VERTEX_DIM 257
#define TILE_N 256

class HeightMap : public iMap
{
public:
	HeightMap ();
	~HeightMap ();

	virtual void Load ( char* szFullPath, D3DXMATRIXA16* pmat );
	virtual bool GetHeight ( IN const float& x, OUT float& y, IN const float& z );
	virtual void Render();

protected:
	GETSET_ADD_REF ( LPDIRECT3DTEXTURE9, texture, Texture );
	D3DMATERIAL9				material;
	LPD3DXMESH					mesh;
	std::vector<D3DXVECTOR3>	verts;


};

