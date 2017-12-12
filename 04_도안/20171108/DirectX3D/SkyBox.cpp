#include "stdafx.h"
#include "SkyBox.h"


SkyBox::SkyBox()
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::Init()
{
	std::vector<D3DXVECTOR3> vertexFormat;

	vertexFormat.push_back(D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
	vertexFormat.push_back(D3DXVECTOR3(-1.0f, 1.0f, -1.0f));
	vertexFormat.push_back(D3DXVECTOR3(1.0f, 1.0f, -1.0f));
	vertexFormat.push_back(D3DXVECTOR3(1.0f, -1.0f, -1.0f));
	vertexFormat.push_back(D3DXVECTOR3(-1.0f, -1.0f, 1.0f));
	vertexFormat.push_back(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	vertexFormat.push_back(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	vertexFormat.push_back(D3DXVECTOR3(1.0f, -1.0f, 1.0f));
	//back
	skyBox.push_back(VertexPT(vertexFormat[0], D3DXVECTOR2(0, 1)));
	skyBox.push_back(VertexPT(vertexFormat[1], D3DXVECTOR2(0, 0)));
	skyBox.push_back(VertexPT(vertexFormat[2], D3DXVECTOR2(1, 0)));

	skyBox.push_back(VertexPT(vertexFormat[0], D3DXVECTOR2(0, 1)));
	skyBox.push_back(VertexPT(vertexFormat[2], D3DXVECTOR2(1, 0)));
	skyBox.push_back(VertexPT(vertexFormat[3], D3DXVECTOR2(1, 1)));
	//front
	skyBox.push_back(VertexPT(vertexFormat[4], D3DXVECTOR2(1, 1)));
	skyBox.push_back(VertexPT(vertexFormat[6], D3DXVECTOR2(0, 0)));
	skyBox.push_back(VertexPT(vertexFormat[5], D3DXVECTOR2(1, 0)));

	skyBox.push_back(VertexPT(vertexFormat[4], D3DXVECTOR2(1, 1)));
	skyBox.push_back(VertexPT(vertexFormat[7], D3DXVECTOR2(0, 1)));
	skyBox.push_back(VertexPT(vertexFormat[6], D3DXVECTOR2(0, 0)));
	//left	
	skyBox.push_back(VertexPT(vertexFormat[4], D3DXVECTOR2(0, 1)));
	skyBox.push_back(VertexPT(vertexFormat[5], D3DXVECTOR2(0, 0)));
	skyBox.push_back(VertexPT(vertexFormat[1], D3DXVECTOR2(1, 0)));
								 
	skyBox.push_back(VertexPT(vertexFormat[4], D3DXVECTOR2(0, 1)));
	skyBox.push_back(VertexPT(vertexFormat[1], D3DXVECTOR2(1, 0)));
	skyBox.push_back(VertexPT(vertexFormat[0], D3DXVECTOR2(1, 1)));
	//right
	skyBox.push_back(VertexPT(vertexFormat[3], D3DXVECTOR2(0, 1)));
	skyBox.push_back(VertexPT(vertexFormat[2], D3DXVECTOR2(0, 0)));
	skyBox.push_back(VertexPT(vertexFormat[6], D3DXVECTOR2(1, 0)));

	skyBox.push_back(VertexPT(vertexFormat[3], D3DXVECTOR2(0, 1)));
	skyBox.push_back(VertexPT(vertexFormat[6], D3DXVECTOR2(1, 0)));
	skyBox.push_back(VertexPT(vertexFormat[7], D3DXVECTOR2(1, 1)));
	//top
	skyBox.push_back(VertexPT(vertexFormat[1], D3DXVECTOR2(0, 0)));
	skyBox.push_back(VertexPT(vertexFormat[5], D3DXVECTOR2(1, 0)));
	skyBox.push_back(VertexPT(vertexFormat[6], D3DXVECTOR2(1, 1)));

	skyBox.push_back(VertexPT(vertexFormat[1], D3DXVECTOR2(0, 0)));
	skyBox.push_back(VertexPT(vertexFormat[6], D3DXVECTOR2(1, 1)));
	skyBox.push_back(VertexPT(vertexFormat[2], D3DXVECTOR2(0, 1)));
	//bottom
	skyBox.push_back(VertexPT(vertexFormat[4], D3DXVECTOR2(1, 1)));
	skyBox.push_back(VertexPT(vertexFormat[0], D3DXVECTOR2(0, 1)));
	skyBox.push_back(VertexPT(vertexFormat[3], D3DXVECTOR2(0, 0)));

	skyBox.push_back(VertexPT(vertexFormat[4], D3DXVECTOR2(1, 1)));
	skyBox.push_back(VertexPT(vertexFormat[3], D3DXVECTOR2(0, 0)));
	skyBox.push_back(VertexPT(vertexFormat[7], D3DXVECTOR2(1, 0)));
	
}

void SkyBox::Render()
{
	D3DXMATRIXA16 skyWorld;
	D3DXMatrixIdentity(&skyWorld);
	D3DXMatrixScaling(&skyWorld, 400.0f, 400.0f, 400.0f);
	D3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DDevice->SetTransform(D3DTS_WORLD, &skyWorld);
	D3DDevice->SetRenderState(D3DRS_CULLMODE, true);
	//Back
	D3DDevice->SetTexture(0, g_pTextureManager->GetTexture("./SkyBox/badomen_bk.png"));
	D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &skyBox[0], sizeof(VertexPT));
	//front
	D3DDevice->SetTexture(0, g_pTextureManager->GetTexture("./SkyBox/badomen_ft.png"));
	D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &skyBox[6], sizeof(VertexPT));
	//left
	D3DDevice->SetTexture(0, g_pTextureManager->GetTexture("./SkyBox/badomen_lf.png"));
	D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &skyBox[12], sizeof(VertexPT));
	//right
	D3DDevice->SetTexture(0, g_pTextureManager->GetTexture("./SkyBox/badomen_rt.png"));
	D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &skyBox[18], sizeof(VertexPT));
	//top
	D3DDevice->SetTexture(0, g_pTextureManager->GetTexture("./SkyBox/badomen_up.png"));
	D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &skyBox[24], sizeof(VertexPT));
	//bottom
	D3DDevice->SetTexture(0, g_pTextureManager->GetTexture("./SkyBox/badomen_dn.png"));
	D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &skyBox[30], sizeof(VertexPT));
}
