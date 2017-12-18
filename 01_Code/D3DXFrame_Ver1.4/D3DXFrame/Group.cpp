#include "stdafx.h"
#include "Group.h"

Group::Group()
	: position( 0.0f, 0.0f, 0.0f )
	, tileCount(0)
{
	D3DXMatrixIdentity( &scaleMat );
	D3DXMatrixIdentity( &rotYMat );
	D3DXMatrixIdentity( &transMat );
	D3DXMatrixIdentity( &worldTM );
}

Group::~Group() {}

void Group::release()
{
	SAFE_RELEASE( materialTexture );
}

void Group::render()
{
	DWORD dwFillMode;
	DEVICE->GetRenderState( D3DRS_FILLMODE, &dwFillMode );

	if ( dwFillMode == D3DFILL_WIREFRAME )
	{
		DEVICE->SetTexture( 0, NULL );
	}
	else
	{
		DEVICE->SetTexture( 0, materialTexture->GetTexture() );
	}

	worldTM = scaleMat * rotYMat * transMat;

	DEVICE->SetTransform( D3DTS_WORLD, &worldTM );
	DEVICE->SetMaterial( &materialTexture->GetMaterial() );
	DEVICE->SetFVF( VertexPNT::FVF );
	DEVICE->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		PNTVerts.size() / 3,
		&PNTVerts[ 0 ],
		sizeof( VertexPNT ) );
}

void Group::setScale( D3DXVECTOR3 pScale )
{
	D3DXMatrixScaling( &scaleMat, pScale.x, pScale.y, pScale.z );
}

void Group::SetRotationY( float pRotY )
{
	D3DXMatrixRotationY( &rotYMat, pRotY );
}

void Group::setPosition( D3DXVECTOR3 pPos )
{
	D3DXMatrixTranslation( &transMat, pPos.x, pPos.y, pPos.z );
}

bool Group::GetHeight( IN const float & x, OUT float & y, IN const float & z )
{
	//float modX = x / 256.0f;
	//float modZ = z / 256.0f;

	//if ( modX < 0 || modZ < 0 || modX > tileCount || modZ > tileCount )
	//{
	//	y = 0.0f;
	//	return false;
	//}

	//if ( x < 0 || z < 0 || x > tileCount || z > tileCount )
	//{
	//	y = 0.0f;
	//	return false;
	//}

	//int intX = ( int )modX;
	//int intZ = ( int )modZ;

	//int intX = ( int )x;
	//int intZ = ( int )z;
	//float dx = x - intX;
	//float dz = z - intZ;
	//
	//int _0 = ( intZ + 0 ) * tileCount + intX + 0;
	//int _1 = ( intZ + 1 ) * tileCount + intX + 0;
	//int _2 = ( intZ + 0 ) * tileCount + intX + 1;
	//int _3 = ( intZ + 1 ) * tileCount + intX + 1;
	//
	//if ( dx + dz < 1.0f ) 
	//{
	//	D3DXVECTOR3 v01 = mapVertexList[ _1 ] - mapVertexList[ _0 ];
	//	D3DXVECTOR3 v02 = mapVertexList[ _2 ] - mapVertexList[ _0 ];
	//
	//	//y = ( mapVertexList[ _0 ].y + ( v01 * dz + v02 * dx ).y ) * 256.0f;
	//	y = mapVertexList[ _0 ].y + ( v01 * dz + v02 * dx ).y;
	//}
	//else
	//{
	//	dx = 1 - dx;
	//	dz = 1 - dz;
	//
	//	D3DXVECTOR3 v31 = mapVertexList[ _1 ] - mapVertexList[ _3 ];
	//	D3DXVECTOR3 v32 = mapVertexList[ _2 ] - mapVertexList[ _3 ];
	//
	//	//y = ( mapVertexList[ _3 ].y + ( v31 * dx + v32 * dz ).y )* 256.0f;
	//	y = mapVertexList[ _3 ].y + ( v31 * dx + v32 * dz ).y;
	//}
	//
	//return true;



	D3DXVECTOR3 vRayOrig( x, 10000, z );

	D3DXVECTOR3 vRayDir( 0, -1, 0 );

	float u, v, d;
	
	for ( size_t i = 0; i < PNTVerts.size(); i += 3 )
	{
		if ( D3DXIntersectTri( &PNTVerts[ i ].p,
			&PNTVerts[ i + 1 ].p,
			&PNTVerts[ i + 2 ].p,
			&vRayOrig,
			&vRayDir, &u, &v, &d ) )
		{
			y = 10000 - d;
			return true;
		}
	}

	return false;
}