#pragma once

class MaterialTexture;

struct Bone : public D3DXFRAME //뼈대이름, 애니메이션 행렬, 그릴(draw) 메쉬데이터, 이웃(형제) 노드, 자식 노드로 구성
{
	//월드 매트릭스를 따로 가지고 있겠다., 즉, worldTM
	D3DXMATRIX CombinedTransformationMatrix;
	bool isRender;
};

struct BoneMesh : public D3DXMESHCONTAINER //그릴 매쉬데이터 -> 매쉬의 이름과 매쉬시, 재질, 이펙트, 스키닝 정보
{
	std::vector<LPDIRECT3DTEXTURE9> vecTexture;
	std::vector<D3DMATERIAL9>		vecMaterial;

	//std::vector<MaterialTexture*>	materialTextureList;
	LPD3DXMESH						orgMesh; //원본 메쉬
	LPD3DXMESH						workMesh; //작업 메쉬 clonemeshFVF로 생성

	D3DXMATRIX**					ppBoneMatrixPtrs;		// 이 메쉬에 영향을 주는 프레임들의 월드 매트릭스 포인터 배열
	D3DXMATRIX*						pBoneOffsetMatrices;	// 원본 메쉬를 로컬스페이스로 보내는 매트릭스들
	//D3DXMATRIX*						pCurrentBoneMatrices;	// 각 본의 계산된 월드 매트릭스 = pBoneOffsetMatrices * ppBoneMatrixPtrs

	DWORD					dwNumPaletteEntries;
	DWORD					dwMaxNumFaceInfls;
	DWORD					dwNumAttrGroups;
	LPD3DXBUFFER			pBufBoneCombos;
};

//커스터마이징을 위해
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