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

	void SetRandomTrackPosition (); // 테스트용

	inline void SetPosition ( D3DXVECTOR3 v )
	{
		m_vPosition = v;
		m_stBoundingSphere.vCenter = v;
	}
	inline SphereInfo* GetBoundingSphere ()
	{
		return &m_stBoundingSphere;
	}

	GETSET( D3DXMATRIX, worldTM, WorldTM );

private:
	SkinnedMesh ();	

	void release();
	void Load ( const char* foldername, const char* filename );
	LPD3DXEFFECT LoadEffect ( char* szFilename );
	void Update ( Bone* frame, D3DXMATRIX* pmatParent );
	void Render ( Bone* frame );
	void SetupBoneMatrixPtrs ( LPD3DXFRAME );

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
	//float						blendTime;
	//float						passedBlendTime;


};

