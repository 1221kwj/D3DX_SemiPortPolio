#include "stdafx.h"
#include "FrustumCulling.h"


FrustumCulling::FrustumCulling()
{
	_vWorldVtx.clear();

	_vWorldVtx.push_back(D3DXVECTOR3(-1, 1, 1));
	_vWorldVtx.push_back(D3DXVECTOR3(1, 1, 1));
	_vWorldVtx.push_back(D3DXVECTOR3(-1, 1, 0));
	_vWorldVtx.push_back(D3DXVECTOR3(1, 1, 0));
	_vWorldVtx.push_back(D3DXVECTOR3(-1, -1, 1));
	_vWorldVtx.push_back(D3DXVECTOR3(1, -1, 1));
	_vWorldVtx.push_back(D3DXVECTOR3(-1, -1, 0));
	_vWorldVtx.push_back(D3DXVECTOR3(1, -1, 0));

	_vPlane.resize(6);
}
FrustumCulling::~FrustumCulling()
{
}

// 뷰포트, 프로젝션
// D3DXVec3Unproject 벡터를 스크린 스페이스에서 로컬 스페이스로 바꿔주는, 스크린 공간으로부터 개체 공간에 벡터를 투영 한다.
// D3DXPlaneFromPoints 
// D3DXPlaneDotCoord  == ax + by  + xz + d*1;
// D3DXPlaneDotCoord(&p, &dphrtrinfo->vCenter > sphereInfo->vRadius)


BOOL FrustumCulling::IsFrustum(SphereInfo * tsphereInfo)
{
	for each(auto p in _vPlane)
	{
		if (D3DXPlaneDotCoord(&p, &tsphereInfo->vCenter) > tsphereInfo->fRadius)
		{
			tsphereInfo->isInFrustum = false;
			return tsphereInfo->isInFrustum;
		}
	}

	tsphereInfo->isInFrustum = true;
	return tsphereInfo->isInFrustum;
}

void FrustumCulling::Update()
{
	std::vector<D3DXVECTOR3> vVtx;
	vVtx.resize( 8 );

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DVIEWPORT9 mViewport;

	D3DDevice->GetTransform(D3DTS_VIEW, &mView);
	D3DDevice->GetTransform(D3DTS_PROJECTION, &mProj);
	D3DDevice->GetViewport(&mViewport);

	for (int i = 0; i < vVtx.size(); ++i)
	{
		//벡터를 스크린 스페이스에서 로컬 스페이스로 바꿔주는, 스크린 공간으로부터 개체 공간에 벡터를 투영 한다.
		D3DXVec3Unproject(&vVtx[i], &_vWorldVtx[i], nullptr, &mProj, &mView, nullptr);
	}
	
	//Left
	D3DXPlaneFromPoints(&_vPlane[0], &vVtx[0], &vVtx[2], &vVtx[4]);
	//Right
	D3DXPlaneFromPoints(&_vPlane[1], &vVtx[3], &vVtx[1], &vVtx[7]);
	//Top
	D3DXPlaneFromPoints(&_vPlane[2], &vVtx[0], &vVtx[1], &vVtx[2]);
	//Bottom
	D3DXPlaneFromPoints(&_vPlane[3], &vVtx[6], &vVtx[7], &vVtx[4]);
	//Near
	D3DXPlaneFromPoints(&_vPlane[4], &vVtx[2], &vVtx[3], &vVtx[7]);
	//Far
	D3DXPlaneFromPoints(&_vPlane[5], &vVtx[1], &vVtx[0], &vVtx[5]);

}
