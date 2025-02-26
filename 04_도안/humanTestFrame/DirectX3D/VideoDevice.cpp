#include "stdafx.h"
#include "VideoDevice.h"


VideoDevice::VideoDevice ()
	: d3d9 ( nullptr )
	, d3dDevice9 ( nullptr )
	, texture ( nullptr )
{
}

VideoDevice::~VideoDevice ()
{


}

void VideoDevice::Init ()
{
	d3d9 = Direct3DCreate9 ( D3D_SDK_VERSION );

	D3DCAPS9	stCaps;
	int			nVertexProcessing;
	d3d9->GetDeviceCaps ( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps );
	if ( stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory ( &stD3DPP, sizeof ( D3DPRESENT_PARAMETERS ) );
	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	stD3DPP.Windowed = TRUE;
	stD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	stD3DPP.EnableAutoDepthStencil = TRUE;
	stD3DPP.AutoDepthStencilFormat = D3DFMT_D16;

	d3d9->CreateDevice ( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		nVertexProcessing,
		&stD3DPP,
		&d3dDevice9 );



	

	D3DXMatrixIdentity ( &worldMatrix );

	d3dDevice9->SetRenderState ( D3DRS_LIGHTING, false );
	//d3dDevice9->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
}

void VideoDevice::RenderBegin ()
{
	d3dDevice9->Clear (
		0,
		nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB ( 128, 235, 244 ),
		1.0f,
		0
	);


	d3dDevice9->BeginScene ();

	d3dDevice9->SetTransform ( D3DTS_WORLD, &worldMatrix );

	// 컬러
	//d3dDevice9->SetFVF ( VertexPC::FVF );
	//////d3dDevice9->DrawPrimitiveUP ( D3DPT_TRIANGLELIST, 1, vertexPCs, sizeof ( VertexPC ) );
	//d3dDevice9->DrawPrimitiveUP ( D3DPT_TRIANGLELIST, 1, ( VertexPC*)&(vertexPCList[0]), sizeof ( VertexPC ) );
	////
	//////d3dDevice9->DrawPrimitiveUP ( D3DPT_LINELIST, 1, vertexPCs, sizeof ( VertexPC ) );

	// 텍스쳐
	d3dDevice9->SetTexture ( 0, texture );
	d3dDevice9->SetFVF ( VertexPT::FVF );
	d3dDevice9->DrawPrimitiveUP ( D3DPT_TRIANGLELIST, 1, vertexPTs, sizeof ( VertexPT ) );





}

void VideoDevice::RenderEnd ()
{
	d3dDevice9->EndScene ();

	d3dDevice9->Present ( nullptr, nullptr, nullptr, nullptr );
}

void VideoDevice::Destroy()
{
	// 일부러 오류냄
	if ( texture != nullptr )
		texture->Release();


	if ( d3dDevice9 != nullptr ) {
		ULONG ul = d3dDevice9->Release();
		//assert( ul == 0 && "unreleased object..." );
	}

	if ( d3d9 != nullptr )
		d3d9->Release();
}

LPDIRECT3DDEVICE9 VideoDevice::GetDevice ()
{
	return d3dDevice9;
}



