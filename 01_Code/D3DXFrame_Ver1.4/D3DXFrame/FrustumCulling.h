#pragma once
class FrustumCulling
{
private:
	vector<D3DXVECTOR3> orgProjVertexList;
	vector<D3DXPLANE>	planeList;

public:
	FrustumCulling();
	~FrustumCulling();

	void update();
	bool IsInFrustum( SphereInfo* );
};

