#include "stdafx.h"
#include "FrustumCulling.h"

FrustumCulling::FrustumCulling()
{
	orgProjVertexList.clear();
	orgProjVertexList.push_back( D3DXVECTOR3( -1.0f,  1.0f,  1.0f ) );
	orgProjVertexList.push_back( D3DXVECTOR3(  1.0f,  1.0f,  1.0f ) );
	orgProjVertexList.push_back( D3DXVECTOR3( -1.0f,  1.0f,  1.0f ) );
	orgProjVertexList.push_back( D3DXVECTOR3(  1.0f,  1.0f,  0.0f ) );
	orgProjVertexList.push_back( D3DXVECTOR3( -1.0f, -1.0f,  1.0f ) );
	orgProjVertexList.push_back( D3DXVECTOR3(  1.0f, -1.0f,  1.0f ) );
	orgProjVertexList.push_back( D3DXVECTOR3( -1.0f, -1.0f,  0.0f ) );
	orgProjVertexList.push_back( D3DXVECTOR3(  1.0f, -1.0f,  0.0f ) );

	planeList.resize( 6 );
}

FrustumCulling::~FrustumCulling() {}

void FrustumCulling::update()
{
	std::vector<D3DXVECTOR3> worldVertexList( 8 );

	D3DXMATRIXA16 matProj, matView;
	DEVICE->GetTransform( D3DTS_PROJECTION, &matProj );
	DEVICE->GetTransform( D3DTS_VIEW,		&matView );

	for ( size_t i = 0; i < worldVertexList.size(); ++i )
	{
		D3DXVec3Unproject( 
			&worldVertexList[ i ],
			&orgProjVertexList[ i ],
			nullptr,
			&matProj,
			&matView,
			nullptr );
	}

	D3DXPlaneFromPoints( &planeList[ 0 ],
		&worldVertexList[ 2 ],
		&worldVertexList[ 3 ],
		&worldVertexList[ 6 ] );

	D3DXPlaneFromPoints( &planeList[ 1 ],
		&worldVertexList[ 1 ],
		&worldVertexList[ 0 ],
		&worldVertexList[ 5 ] );

	D3DXPlaneFromPoints( &planeList[ 2 ],
		&worldVertexList[ 0 ],
		&worldVertexList[ 2 ],
		&worldVertexList[ 4 ] );

	D3DXPlaneFromPoints( &planeList[ 3 ],
		&worldVertexList[ 3 ],
		&worldVertexList[ 1 ],
		&worldVertexList[ 7 ] );

	D3DXPlaneFromPoints( &planeList[ 4 ],
		&worldVertexList[ 0 ],
		&worldVertexList[ 1 ],
		&worldVertexList[ 2 ] );

	D3DXPlaneFromPoints( &planeList[ 5 ],
		&worldVertexList[ 6 ],
		&worldVertexList[ 7 ],
		&worldVertexList[ 4 ] );
}

bool FrustumCulling::IsInFrustum( SphereInfo * sphereInfo )
{
	for each( D3DXPLANE p in planeList )
	{
		if ( D3DXPlaneDotCoord( &p, &sphereInfo->vCenter ) > sphereInfo->fRadius )
		{
			sphereInfo->isInFrustum = false;
			return false;
		}
	}
	sphereInfo->isInFrustum = true;
	return true;
}
