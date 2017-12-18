#include "stdafx.h"
#include "Pyramid.h"


Pyramid::Pyramid ()
{
}


Pyramid::~Pyramid ()
{
}

void Pyramid::Init ( D3DXCOLOR color, D3DXMATRIXA16& worldMat)
{
	D3DXMatrixIdentity(&worldMatrix);
	

	D3DXVECTOR3 normal ( 0, 0, 0 );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( 0, 0, 0 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( 1, -1, -1 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( -1, -1, -1 ), normal ) );

	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( 0, 0, 0 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( 1, -1, 1 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( 1, -1, -1 ), normal ) );

	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( 0, 0, 0 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( -1, -1, 1 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( 1, -1, 1 ), normal ) );

	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( 0, 0, 0 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( -1, -1, -1 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( -1, -1, 1 ), normal ) );

	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( 1, -1, -1 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( -1, -1, 1 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( -1, -1, -1 ), normal ) );

	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( 1, -1, -1 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( 1, -1, 1 ), normal ) );
	vertexList.push_back ( VertexPN ( D3DXVECTOR3 ( -1, -1, 1 ), normal ) );

	

	for ( int i = 0; i < vertexList.size (); i += 3 ) {
		D3DXVECTOR3 v01 = vertexList[i + 1].p - vertexList[i + 0].p;
		D3DXVECTOR3 v02 = vertexList[i + 2].p - vertexList[i + 0].p;
		D3DXVec3Cross ( &normal, &v01, &v02 );

		vertexList[i].n = vertexList[i + 1].n = vertexList[i + 2].n = normal;
	}

	

	ZeroMemory ( &material, sizeof ( D3DMATERIAL9 ) );
	material.Ambient = material.Diffuse = material.Specular = color;

	

	//for (int i = 0; i < vertexList.size(); i++)
	//{
	//	D3DXVec3TransformCoord(&vertexList[i].p, &vertexList[i].p, &worldMat);
	//}

	transVertexList = vertexList;
	//
}

void Pyramid::Render (D3DXMATRIXA16& worldMat)
{
	for (int i = 0; i < vertexList.size(); i++)
	{
		D3DXVec3TransformCoord(&transVertexList[i].p, &vertexList[i].p, &worldMat);
	}

	//worldMatrix = _worldMatrix;
	DEVICE->SetMaterial ( &material );
	//DEVICE->SetRenderState ( D3DRS_LIGHTING, true );
	
	//DEVICE->SetTransform ( D3DTS_WORLD, &worldMatrix);
	DEVICE->SetFVF ( VertexPN::FVF );
	DEVICE->DrawPrimitiveUP (
		D3DPT_TRIANGLELIST,
		transVertexList.size () / 3,
		&transVertexList[0],
		sizeof ( VertexPN )
	);

	D3DXMATRIXA16 idMat;
	D3DXMatrixIdentity(&idMat);
	DEVICE->SetTransform(D3DTS_WORLD, &idMat);
}
