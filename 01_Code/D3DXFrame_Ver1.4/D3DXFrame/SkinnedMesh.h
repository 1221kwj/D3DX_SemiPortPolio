#pragma once

struct Bone;

class SkinnedMesh
{
	friend class SkinnedMeshManager;

public:
	SkinnedMesh ( char* skinName, char* szFolder, char* szFilename );
	~SkinnedMesh ( void );

	void UpdateAndRender ();
	void SetAnimationIndex ( int nIndex );
	void SetAnimationIndex(int index, bool isBlend);
	void SetRandomTrackPosition (); // 테스트용
	D3DXMATRIX GetCombineMatrix( char* frameName );
	D3DXMATRIX GetRotateMatrix() { return rotateMat; }

	inline void SetPosition ( D3DXVECTOR3 v )
	{
		m_vPosition = v;
		m_stBoundingSphere.vCenter = v;
	}
	inline SphereInfo* GetBoundingSphere ()
	{
		return &m_stBoundingSphere;
	}

	LPD3DXFRAME GetRoot();

	GETSET( float, angle, Angle );
	string GetName() { return fullName; }

private:
	SkinnedMesh ();	

	void release();
	void Load ( const char* foldername, const char* filename );
	LPD3DXEFFECT LoadEffect ( char* szFilename );
	void Update ( Bone* frame, D3DXMATRIX* pmatParent );
	void Render ( Bone* frame );
	void SetupBoneMatrixPtrs ( LPD3DXFRAME );

	//void SetMatrices(IN LPD3DXFRAME frame, IN LPSTR boneName);
	//void SetBone(IN LPD3DXFRAME frame, IN LPSTR boneName, IN LPSTR rootBoneName);

	//void UpdateSkinnedMesh ( LPD3DXFRAME );	
	void Destroy ();

	
private: 
	// 하나만 생성
	Bone*						root;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	SphereInfo					m_stBoundingSphere;

	// 객체마다 생성
	LPD3DXANIMATIONCONTROLLER	animationController;
	D3DXVECTOR3					m_vPosition;
	//중간에 겹치는 시간
	float						blendTime = 0.2f;
	//어느정도 지나갔는지에 대한 시간
	float						passedBlendTime = 0.0f;
	D3DXMATRIX					rotateMat;
	string						fullName;
};

