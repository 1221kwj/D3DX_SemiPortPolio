#pragma once

class MaterialTexture;

struct Bone : public D3DXFRAME //�����̸�, �ִϸ��̼� ���, �׸�(draw) �޽�������, �̿�(����) ���, �ڽ� ���� ����
{
	//���� ��Ʈ������ ���� ������ �ְڴ�., ��, worldTM
	D3DXMATRIX CombinedTransformationMatrix;
	bool isRender;
};

struct BoneMesh : public D3DXMESHCONTAINER //�׸� �Ž������� -> �Ž��� �̸��� �Ž���, ����, ����Ʈ, ��Ű�� ����
{
	std::vector<LPDIRECT3DTEXTURE9> vecTexture;
	std::vector<D3DMATERIAL9>		vecMaterial;

	//std::vector<MaterialTexture*>	materialTextureList;
	LPD3DXMESH						orgMesh; //���� �޽�
	LPD3DXMESH						workMesh; //�۾� �޽� clonemeshFVF�� ����

	D3DXMATRIX**					ppBoneMatrixPtrs;		// �� �޽��� ������ �ִ� �����ӵ��� ���� ��Ʈ���� ������ �迭
	D3DXMATRIX*						pBoneOffsetMatrices;	// ���� �޽��� ���ý����̽��� ������ ��Ʈ������
	//D3DXMATRIX*						pCurrentBoneMatrices;	// �� ���� ���� ���� ��Ʈ���� = pBoneOffsetMatrices * ppBoneMatrixPtrs

	DWORD					dwNumPaletteEntries;
	DWORD					dwMaxNumFaceInfls;
	DWORD					dwNumAttrGroups;
	LPD3DXBUFFER			pBufBoneCombos;
};

//Ŀ���͸���¡�� ����
class AllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	AllocateHierarchy();
	~AllocateHierarchy();

	GETSET_REF(std::string, folder, Folder);
	GETSET(D3DXVECTOR3, minVector, MinVector);
	GETSET(D3DXVECTOR3, maxVector, MaxVector);
	GETSET(DWORD, defaultPaletteSize, DefaultPaletteSize);
	GETSET(DWORD, maxPaletteSize, MaxPaletteSize);

	STDMETHODIMP CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame);
	STDMETHODIMP CreateMeshContainer(LPCSTR Name
		, CONST D3DXMESHDATA * pMeshData
		, CONST D3DXMATERIAL * pMaterials
		, CONST D3DXEFFECTINSTANCE * pEffectInstances
		, DWORD NumMaterials, CONST DWORD * pAdjacency
		, LPD3DXSKININFO pSkinInfo
		, LPD3DXMESHCONTAINER * ppNewMeshContainer);
	STDMETHODIMP DestroyFrame(LPD3DXFRAME pFrameToFree);
	STDMETHODIMP DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree);
private:

};