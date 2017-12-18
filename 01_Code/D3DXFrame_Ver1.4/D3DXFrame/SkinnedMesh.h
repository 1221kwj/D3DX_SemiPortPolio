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
	
	void SetRandomTrackPosition (); // �׽�Ʈ��
	D3DXMATRIX GetCombineMatrix( char* frameName );

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
	// �ϳ��� ����
	Bone*						root;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	SphereInfo					m_stBoundingSphere;

	// ��ü���� ����
	LPD3DXANIMATIONCONTROLLER	animationController;
	D3DXVECTOR3					m_vPosition;
	//float						blendTime;
	//float						passedBlendTime;
};

