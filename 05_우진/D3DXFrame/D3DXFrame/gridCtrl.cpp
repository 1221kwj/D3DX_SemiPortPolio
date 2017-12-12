#include "stdafx.h"
#include "gridCtrl.h"

gridCtrl::gridCtrl()
	: isRender( true )
{
}

gridCtrl::~gridCtrl() {}

void gridCtrl::init( void )
{
	int lineCount = 30 / 2;
	float interval = 1.0f;
	float max = lineCount * interval;
	D3DCOLOR	color;

	color = D3DCOLOR_XRGB( 255, 0, 0 );
	vertexList.push_back( VertexPC( D3DXVECTOR3( -max, 0.0f, 0.0f ), D3DCOLOR( color ) ) );
	vertexList.push_back( VertexPC( D3DXVECTOR3( max, 0.0f, 0.0f ), D3DCOLOR( color ) ) );

	color = D3DCOLOR_XRGB( 0, 255, 0 );
	vertexList.push_back( VertexPC( D3DXVECTOR3( 0.0f, -max, 0.0f ), D3DCOLOR( color ) ) );
	vertexList.push_back( VertexPC( D3DXVECTOR3( 0.0f, max, 0.0f ), D3DCOLOR( color ) ) );

	color = D3DCOLOR_XRGB( 0, 0, 255 );
	vertexList.push_back( VertexPC( D3DXVECTOR3( 0.0f, 0.0f, -max ), D3DCOLOR( color ) ) );
	vertexList.push_back( VertexPC( D3DXVECTOR3( 0.0f, 0.0f, max ), D3DCOLOR( color ) ) );

	for ( int i = 1; i <= lineCount; ++i ) {

		color = ( i % 5 == 0 ) ? D3DCOLOR_XRGB( 255, 255, 255 ) : D3DCOLOR_XRGB( 128, 128, 128 );
		vertexList.push_back( VertexPC( D3DXVECTOR3( -max, 0.0f, i ), D3DCOLOR( color ) ) );
		vertexList.push_back( VertexPC( D3DXVECTOR3( max, 0.0f, i ), D3DCOLOR( color ) ) );

		vertexList.push_back( VertexPC( D3DXVECTOR3( -max, 0.0f, -i ), D3DCOLOR( color ) ) );
		vertexList.push_back( VertexPC( D3DXVECTOR3( max, 0.0f, -i ), D3DCOLOR( color ) ) );

		vertexList.push_back( VertexPC( D3DXVECTOR3( i, 0.0f, -max ), D3DCOLOR( color ) ) );
		vertexList.push_back( VertexPC( D3DXVECTOR3( i, 0.0f, max ), D3DCOLOR( color ) ) );

		vertexList.push_back( VertexPC( D3DXVECTOR3( -i, 0.0f, -max ), D3DCOLOR( color ) ) );
		vertexList.push_back( VertexPC( D3DXVECTOR3( -i, 0.0f, max ), D3DCOLOR( color ) ) );
	}
}

void gridCtrl::release( void )
{
}

void gridCtrl::render( void )
{
	if ( isRender )
	{
		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity( &mat );

		DEVICE->SetRenderState( D3DRS_LIGHTING, false );
		DEVICE->SetTexture( 0, NULL );
		DEVICE->SetTransform( D3DTS_WORLD, &mat );
		DEVICE->SetFVF( VertexPC::FVF );
		DEVICE->DrawPrimitiveUP(
			D3DPT_LINELIST,
			vertexList.size() / 2,
			&( vertexList[ 0 ] ),
			sizeof( VertexPC )
		);
	}
}
