#pragma once
class RayCtrl
{
private:
	enum RaySpaceType
	{
		NONE,
		VIEW,
		WORLD
	};

	RaySpaceType raySpaceType;

public:
	RayCtrl();
	~RayCtrl();

	static RayCtrl RayAtViewSpace( int x, int y );
	static RayCtrl RayAtWorldSpace( int x, int y );
	bool IsPicked( SphereInfo* sphereInfo );

	D3DXVECTOR3 orgPosition;
	D3DXVECTOR3 direction;
};

