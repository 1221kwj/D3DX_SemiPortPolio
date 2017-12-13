#include "stdafx.h"

RayCtrl::RayCtrl()
	: orgPosition(0,0,0)
	, direction(0,0,0)
	, raySpaceType(NONE)
{
}

RayCtrl::~RayCtrl()
{
}
//ViewSpace
RayCtrl RayCtrl::RayAtViewSpace(int x, int y)
{
	//view port
	D3DVIEWPORT9 vp;
	D3DDevice->GetViewport(&vp);
	//projection matrix 
	D3DXMATRIXA16 proMat;
	D3DDevice->GetTransform(D3DTS_PROJECTION, &proMat);

	//받아온 마우스 값으로 view ray
	RayCtrl r;
  	r.direction.x = ((2.0f * x) / vp.Width - 1.0f) / proMat._11;
	r.direction.y = ((-2.0f * y) / vp.Height + 1.0f) / proMat._22;
	r.direction.z = 1.0f;

	//debug check use
	r.raySpaceType = VIEW;

	return r;
}
//WorldSpace
RayCtrl RayCtrl::RayAtWorldSpace(int x, int y)
{
	//위에 값을 가져옴
	RayCtrl r = RayAtViewSpace(x, y);

	D3DXMATRIXA16 viewMat, IviewMat;
	//view matrix
	D3DDevice->GetTransform(D3DTS_VIEW, &viewMat);
	//view matrix inverse
	D3DXMatrixInverse(&IviewMat, 0, &viewMat);

	D3DXVec3TransformCoord(&r.orgPosition, &r.orgPosition, &IviewMat);
	D3DXVec3TransformNormal(&r.direction, &r.direction, &IviewMat);
	D3DXVec3Normalize(&r.direction, &r.direction);
	//debug check use
	r.raySpaceType = WORLD;

	return r;
}

bool RayCtrl::IsPicked(SphereInfo * sphereInfo)
{
	
	D3DXVECTOR3 localOrg = orgPosition - sphereInfo->vCenter;
	//a
	float vv = D3DXVec3Dot(&direction, &direction);
	//b
	float qv = D3DXVec3Dot(&localOrg, &direction);
	//c = qq - rr 
	float qq = D3DXVec3Dot(&localOrg, &localOrg);
	float rr = sphereInfo->fRadius * sphereInfo->fRadius;

	//q*q + 2q*vt + v*v*t*t = r*r;
	//vvT^2 + 2qvT + qq - rr;
	//a = vv, b = 2qv, c = qq -rr
	//위에 b가 2의 배수이기 때문에 b' = b /2 = qv
	//a^2 - b'*c 라는 짝수 근의 공식을 적용 시킬수 있음
	//그러므로 대입하면, qv * qv - vv * (qq - rr);

	return qv * qv - vv * (qq - rr) >= 0;

}
