#pragma once

class MaterialTexture;

struct Bone : public D3DXFRAME
{
	D3DXMATRIX  CombinedTransformationMatrix;
	bool		isRender;
};

struct BoneMesh : public D3DXMESHCONTAINER
{
	std::vector<LPDIRECT3DTEXTURE9> vecTexture;
	std::vector<D3DMATERIAL9>		vecMaterial;


	LPD3DXMESH				orgMesh;				// ���� �޽�
	LPD3DXMESH				workMesh;				// �۾� �޽� CloneMeshFVF�� ����
	D3DXMATRIX**			ppBoneMatrixPtrs;		// �� �޽��� ������ �ִ� ������'��'�� �����Ʈ���� ������ �迭
	D3DXMATRIX*				pBoneOffsetMatrices;	// ���� �޽��� ���ý����̽��� ������ ��Ʈ������.
													//D3DXMATRIX* pCurrentBoneMatrices;
													// �� ���� ���� �����Ʈ����
	DWORD					dwNumPaletteEntries;
	DWORD					dwMaxNumFaceInfls;
	DWORD					dwNumAttrGroups;
	LPD3DXBUFFER			pBufBoneCombos;

};

class AllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	AllocateHierarchy();
	~AllocateHierarchy();

	GETSET( std::string, folder, Folder );
	GETSET( D3DXVECTOR3, minVector, MinVector );
	GETSET( D3DXVECTOR3, maxVector, MaxVector );
	GETSET( DWORD, defaultPaletteSize, DefaultPaletteSize );
	GETSET( DWORD, maxPaletteSize, MaxPaletteSize );
	
	STDMETHOD( CreateFrame )( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame ) override;
	
	STDMETHOD( CreateMeshContainer )( 
		THIS_ LPCSTR				Name,
		CONST D3DXMESHDATA*			pMeshData,
		CONST D3DXMATERIAL*			pMaterials,
		CONST D3DXEFFECTINSTANCE*	pEffectInstances,
		DWORD						NumMaterials,
		CONST DWORD*				pAdjacency,
		LPD3DXSKININFO				pSkinInfo,
		LPD3DXMESHCONTAINER*		ppNewMeshContainer ) override;
	
	STDMETHOD( DestroyFrame )( THIS_ LPD3DXFRAME pFrameToFree ) override;
	
	STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree ) override;
};

