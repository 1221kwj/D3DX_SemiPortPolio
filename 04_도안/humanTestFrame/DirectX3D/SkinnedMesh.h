#pragma once

struct Bone;

class SkinnedMesh
{
	friend class SkinnedMeshManager;

public:
	SkinnedMesh (char* szFolder, char* szFilename);
	~SkinnedMesh (void);

	void UpdateAndRender(void);
	void SetAnimationIndex(int nIndex);

	void SetRandomTrackPosition(); // 테스트용

	inline void SetPosition(D3DXVECTOR3 v)
	{
		m_vPosition = v;
		m_stBoundingSphere.vCenter = v;
	}
	inline SphereInfo* GetBoundingSphere()
	{
		return &m_stBoundingSphere;
	}

	Bone* GetRoot() { return root; }
	LPD3DXANIMATIONCONTROLLER GetAnicontroller() { return animationController; }
	void SetRoot(Bone* temp) { root = temp; }
	void SetTransformMat(LPCSTR name, D3DXMATRIX mat);
	D3DXVECTOR3 GetPosition() { return m_vPosition; }

private:
	SkinnedMesh();

	void Load(const char* foldername, const char* filename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(Bone* frame, D3DXMATRIX* pmatParent);
	void Render(Bone* frame);
	void SetupBoneMatrixPtrs(LPD3DXFRAME);
	void SetMatrices(IN LPD3DXFRAME frame, IN LPSTR boneName);
	void SetBone(IN LPD3DXFRAME frame, IN LPSTR boneName, IN LPSTR rootBoneName);

	//void UpdateSkinnedMesh ( LPD3DXFRAME );	
	void Destroy();
private:
	/*
	LPD3DXFRAME					root;	
	
	LPD3DXANIMATIONCONTROLLER	aniController = nullptr;

	D3DXMATRIX* mat;
	//중간에 겹치는 시간
	float blendTime = 0.2f;
	//어느정도 지나갔는지에 대한 시간
	float passedBlendTime = 0.0f;
	*/
	// 하나만 생성
	Bone*						root;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	SphereInfo					m_stBoundingSphere;

	// 객체마다 생성
	LPD3DXANIMATIONCONTROLLER	animationController;
	D3DXVECTOR3					m_vPosition;
	D3DXMATRIX					transMat;
	//float						blendTime;
	//float						passedBlendTime;
};

