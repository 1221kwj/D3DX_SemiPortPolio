#include "stdafx.h"
#include "skyBox.h"

skyBox:: skyBox() {}
skyBox::~skyBox() {}

HRESULT skyBox::init()
{
	vector<D3DXVECTOR3> vertexFormat;

	vertexFormat.push_back( D3DXVECTOR3( -1.0f, -1.0f, -1.0f ) );
	vertexFormat.push_back( D3DXVECTOR3( -1.0f, 1.0f, -1.0f ) );
	vertexFormat.push_back( D3DXVECTOR3( 1.0f, 1.0f, -1.0f ) );
	vertexFormat.push_back( D3DXVECTOR3( 1.0f, -1.0f, -1.0f ) );
	vertexFormat.push_back( D3DXVECTOR3( -1.0f, -1.0f, 1.0f ) );
	vertexFormat.push_back( D3DXVECTOR3( -1.0f, 1.0f, 1.0f ) );
	vertexFormat.push_back( D3DXVECTOR3( 1.0f, 1.0f, 1.0f ) );
	vertexFormat.push_back( D3DXVECTOR3( 1.0f, -1.0f, 1.0f ) );
	//back
	skyBoxList.push_back( VertexPT( vertexFormat[ 0 ], D3DXVECTOR2( 0, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 1 ], D3DXVECTOR2( 0, 0 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 2 ], D3DXVECTOR2( 1, 0 ) ) );

	skyBoxList.push_back( VertexPT( vertexFormat[ 0 ], D3DXVECTOR2( 0, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 2 ], D3DXVECTOR2( 1, 0 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 3 ], D3DXVECTOR2( 1, 1 ) ) );
	//front
	skyBoxList.push_back( VertexPT( vertexFormat[ 4 ], D3DXVECTOR2( 1, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 6 ], D3DXVECTOR2( 0, 0 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 5 ], D3DXVECTOR2( 1, 0 ) ) );

	skyBoxList.push_back( VertexPT( vertexFormat[ 4 ], D3DXVECTOR2( 1, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 7 ], D3DXVECTOR2( 0, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 6 ], D3DXVECTOR2( 0, 0 ) ) );
	//left	
	skyBoxList.push_back( VertexPT( vertexFormat[ 4 ], D3DXVECTOR2( 0, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 5 ], D3DXVECTOR2( 0, 0 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 1 ], D3DXVECTOR2( 1, 0 ) ) );

	skyBoxList.push_back( VertexPT( vertexFormat[ 4 ], D3DXVECTOR2( 0, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 1 ], D3DXVECTOR2( 1, 0 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 0 ], D3DXVECTOR2( 1, 1 ) ) );
	//right
	skyBoxList.push_back( VertexPT( vertexFormat[ 3 ], D3DXVECTOR2( 0, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 2 ], D3DXVECTOR2( 0, 0 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 6 ], D3DXVECTOR2( 1, 0 ) ) );

	skyBoxList.push_back( VertexPT( vertexFormat[ 3 ], D3DXVECTOR2( 0, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 6 ], D3DXVECTOR2( 1, 0 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 7 ], D3DXVECTOR2( 1, 1 ) ) );
	//top
	skyBoxList.push_back( VertexPT( vertexFormat[ 1 ], D3DXVECTOR2( 0, 0 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 5 ], D3DXVECTOR2( 1, 0 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 6 ], D3DXVECTOR2( 1, 1 ) ) );

	skyBoxList.push_back( VertexPT( vertexFormat[ 1 ], D3DXVECTOR2( 0, 0 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 6 ], D3DXVECTOR2( 1, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 2 ], D3DXVECTOR2( 0, 1 ) ) );
	//bottom
	skyBoxList.push_back( VertexPT( vertexFormat[ 4 ], D3DXVECTOR2( 1, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 0 ], D3DXVECTOR2( 0, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 3 ], D3DXVECTOR2( 0, 0 ) ) );

	skyBoxList.push_back( VertexPT( vertexFormat[ 4 ], D3DXVECTOR2( 1, 1 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 3 ], D3DXVECTOR2( 0, 0 ) ) );
	skyBoxList.push_back( VertexPT( vertexFormat[ 7 ], D3DXVECTOR2( 1, 0 ) ) );


	return S_OK;
}

void skyBox::render()
{
	D3DXMATRIXA16 skyWorld;
	D3DXMatrixIdentity( &skyWorld );
	D3DXMatrixScaling( &skyWorld, 400.0f, 400.0f, 400.0f );
	DEVICE->SetRenderState( D3DRS_LIGHTING, false );
	DEVICE->SetRenderState( D3DRS_CULLMODE, true );
	DEVICE->SetTransform( D3DTS_WORLD, &skyWorld );
	DEVICE->SetFVF( VertexPT::FVF );
	//Back
	DEVICE->SetTexture( 0, TEXTUREMANAGER->GetTexture( "./SkyBox/badomen_bk.png" ) );
	DEVICE->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, &skyBoxList[ 0 ], sizeof( VertexPT ) );
	//front
	DEVICE->SetTexture( 0, TEXTUREMANAGER->GetTexture( "./SkyBox/badomen_ft.png" ) );
	DEVICE->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, &skyBoxList[ 6 ], sizeof( VertexPT ) );
	//left
	LPDIRECT3DTEXTURE9 a = TEXTUREMANAGER->GetTexture( "./SkyBox/badomen_lf.png" );

	DEVICE->SetTexture( 0, TEXTUREMANAGER->GetTexture( "./SkyBox/badomen_lf.png" ) );
	DEVICE->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, &skyBoxList[ 12 ], sizeof( VertexPT ) );
	//right
	DEVICE->SetTexture( 0, TEXTUREMANAGER->GetTexture( "./SkyBox/badomen_rt.png" ) );
	DEVICE->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, &skyBoxList[ 18 ], sizeof( VertexPT ) );
	//top
	DEVICE->SetTexture( 0, TEXTUREMANAGER->GetTexture( "./SkyBox/badomen_up.png" ) );
	DEVICE->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, &skyBoxList[ 24 ], sizeof( VertexPT ) );
	//bottom
	DEVICE->SetTexture( 0, TEXTUREMANAGER->GetTexture( "./SkyBox/badomen_dn.png" ) );
	DEVICE->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, &skyBoxList[ 30 ], sizeof( VertexPT ) );
}
