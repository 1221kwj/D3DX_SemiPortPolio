#pragma once
class FrustumCulling
{

public:
	FrustumCulling();
	~FrustumCulling();

	BOOL IsFrustum(SphereInfo* tsphereInfo);
	void Update();

private:
	std::vector<D3DXPLANE> _vPlane;
	std::vector<D3DXVECTOR3> _vWorldVtx;
};

