#pragma once
#include "singletonBase.h"
#include "VideoDevice.h"

struct VertexPC
{
	VertexPC( D3DXVECTOR3 _p, D3DCOLOR _c )
		: p( _p )
		, c( _c )
	{}

	VertexPC() {}

	D3DXVECTOR3 p;
	D3DCOLOR c;

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE, };
};

struct VertexPN
{
	VertexPN() {}
	VertexPN(D3DXVECTOR3 _p, D3DXVECTOR3 _n)
		: p(_p)
		, n(_n) 
	{}

	D3DXVECTOR3 p;
	D3DXVECTOR3	n;

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL, };
};

struct VertexPT
{
	VertexPT( D3DXVECTOR3 _p, D3DXVECTOR2 _t )
		: p( _p )
		, t( _t )
	{}

	VertexPT() {}

	D3DXVECTOR3 p;
	D3DXVECTOR2 t;

	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1, };
};

struct VertexPNT
{
	VertexPNT() {}
	VertexPNT(D3DXVECTOR3 _p, D3DXVECTOR3 _n, D3DXVECTOR2 _t) 
		: p(_p)
		, n(_n)
		, t(_t) 
	{}

	D3DXVECTOR3 p;
	D3DXVECTOR3	n;
	D3DXVECTOR2	t;

	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, };
};

struct VertexRHWC
{
	VertexRHWC( D3DXVECTOR4 _r, D3DCOLOR _c )
		: r( _r )
		, c( _c )
	{}
	VertexRHWC() {}

	D3DXVECTOR4 r;
	D3DCOLOR c;

	enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE, };
};

class deviceManager : public singletonBase <deviceManager>
{
private:
	VideoDevice*	vd;

public:
	deviceManager();
	~deviceManager();

	void init( void );
	void release( void );
	void renderBegin( void );
	void renderEnd( void );
	
	LPDIRECT3DDEVICE9 GetDevice();
};

