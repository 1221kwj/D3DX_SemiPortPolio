#include "stdafx.h"

#include "AllocateHierarchy.h"
#include "MatrialTexture.h"

#include "SkinnedMesh.h"



SkinnedMesh::SkinnedMesh()
	: root(nullptr)
	, animationController(nullptr)
	//, blendTime (0.3f)
	//, passedBlendTime(0.0f)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_vPosition(0, 0, 0)
{
}

SkinnedMesh::SkinnedMesh(char* szFolder, char* szFilename)
	: root(NULL)
	, animationController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_vPosition(0, 0, 0)
{
	SkinnedMesh* skinnedMesh = g_pSkinnedMeshManager->GetSkinnedMesh(szFolder, szFilename);

	root = skinnedMesh->root;
	m_dwWorkingPaletteSize = skinnedMesh->m_dwWorkingPaletteSize;
	m_pmWorkingPalette = skinnedMesh->m_pmWorkingPalette;
	m_pEffect = skinnedMesh->m_pEffect;
	m_stBoundingSphere = skinnedMesh->m_stBoundingSphere;

	skinnedMesh->animationController->CloneAnimationController(
		skinnedMesh->animationController->GetMaxNumAnimationOutputs(),
		skinnedMesh->animationController->GetMaxNumAnimationSets(),
		skinnedMesh->animationController->GetMaxNumTracks(),
		skinnedMesh->animationController->GetMaxNumEvents(),
		&animationController);
}

SkinnedMesh::~SkinnedMesh()
{
	//AllocateHierarchy alloc;
	//D3DXFrameDestroy(root, &alloc);
	SAFE_RELEASE(animationController);
}

void SkinnedMesh::Load(const char * foldername, const char * filename)
{
	m_pEffect = LoadEffect("MultiAnimation.fx");

	D3DXMatrixIdentity(&transMat);

	int nPaletteSize = 0;
	m_pEffect->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

	std::string folder(foldername);
	std::string fullpath = foldername + std::string("\\") + filename;

	AllocateHierarchy alloc;
	alloc.SetFolder(folder);
	alloc.SetDefaultPaletteSize(nPaletteSize);

	m_stBoundingSphere.vCenter = (alloc.GetMinVector() + alloc.GetMaxVector()) / 2.0f;
	m_stBoundingSphere.fRadius = D3DXVec3Length(&(alloc.GetMinVector() - alloc.GetMaxVector()));

	D3DXLoadMeshHierarchyFromXA(
		fullpath.c_str(),
		D3DXMESH_MANAGED,
		D3DDevice,
		&alloc,
		nullptr,
		(LPD3DXFRAME*)&root,
		&animationController
	);

	if (m_pmWorkingPalette)
		delete[] m_pmWorkingPalette;

	m_dwWorkingPaletteSize = alloc.GetMaxPaletteSize();
	m_pmWorkingPalette = new D3DXMATRIX[m_dwWorkingPaletteSize];
	if (m_pmWorkingPalette == NULL)
	{
		m_dwWorkingPaletteSize = 0;
	}

	if (root)
		SetupBoneMatrixPtrs(root);

}

void SkinnedMesh::Update(Bone* frame, D3DXMATRIX* pmatParent)
{
	frame->CombinedTransformationMatrix = frame->TransformationMatrix;
	if (pmatParent)
	{
		frame->CombinedTransformationMatrix =
			frame->CombinedTransformationMatrix * (*pmatParent);
	}

	if (frame->pFrameSibling)
	{
		Update((Bone*)frame->pFrameSibling, pmatParent);
	}

	if (frame->pFrameFirstChild)
	{
		Update((Bone*)frame->pFrameFirstChild, &(frame->CombinedTransformationMatrix));
	}
}


void SkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME frame)
{
	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
	// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
	// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
	// 찾아서 월드매트릭스를 걸어줘라.
	if (frame == nullptr)
		return;
	Bone* bone = (Bone*)frame;
	if (bone->pMeshContainer)
	{
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;
		if (boneMesh->pSkinInfo)
		{
			LPD3DXSKININFO skinInfo = boneMesh->pSkinInfo;
			DWORD dwNumBones = boneMesh->pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR szBoneName = skinInfo->GetBoneName(i);
				if (szBoneName == NULL || strlen(szBoneName) == 0)
					continue;

				Bone* p = (Bone*)D3DXFrameFind(root, skinInfo->GetBoneName(i));
				boneMesh->ppBoneMatrixPtrs[i] = &(p->CombinedTransformationMatrix);
			}
		}

	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (frame->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs(frame->pFrameFirstChild);
	}

	if (frame->pFrameSibling)
	{
		SetupBoneMatrixPtrs(frame->pFrameSibling);
	}

}

void SkinnedMesh::SetMatrices(IN LPD3DXFRAME frame, IN LPSTR boneName)
{
	Bone* bone = (Bone*)D3DXFrameFind(frame, boneName);
	transMat = bone->CombinedTransformationMatrix;
}

void SkinnedMesh::SetBone(IN LPD3DXFRAME frame, IN LPSTR boneName, IN LPSTR rootBoneName)
{
	Bone* bone = (Bone*)D3DXFrameFind(frame, boneName);
	Bone* rootBone = (Bone*)D3DXFrameFind(root, rootBoneName);

	rootBone->CombinedTransformationMatrix = bone->CombinedTransformationMatrix;
}

void SkinnedMesh::UpdateAndRender()
{
	if (animationController)
	{
		animationController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);
	}

	if (root)
	{
		D3DXMATRIXA16 mat;
		D3DXMatrixTranslation(&mat, m_vPosition.x, m_vPosition.y, m_vPosition.z);

		Update(root, &mat);
		//Update(root, &transMat);

		Render(root);
	}


}

void SkinnedMesh::Render(Bone* pBone)
{
	assert(pBone);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if (pBone->pMeshContainer)
	{
		BoneMesh* pBoneMesh = (BoneMesh*)pBone->pMeshContainer;

		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos =
			(LPD3DXBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

		D3DXMATRIXA16 matViewProj, matView, matProj;	
		D3DDevice->GetTransform(D3DTS_VIEW, &matView);
		D3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
		matViewProj = matView * matProj;

		D3DXMATRIXA16 mView, mInvView;
		D3DDevice->GetTransform(D3DTS_VIEW, &mView);
		D3DXMatrixInverse(&mInvView, 0, &mView);
		D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

		// for each palette
		for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
		{
			// set each transform into the palette
			for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
			{
				DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
				if (dwMatrixIndex != UINT_MAX)
				{
					m_pmWorkingPalette[dwPalEntry] =
						pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
						(*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
				}
			}

			// set the matrix palette into the effect
			m_pEffect->SetMatrixArray("amPalette",
				m_pmWorkingPalette,
				pBoneMesh->dwNumPaletteEntries);

			m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
			m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			m_pEffect->SetVector("vWorldLightPos", &D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f));
			m_pEffect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
			m_pEffect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.53f, 0.53f, 0.53f, 0.53f));
			m_pEffect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here
			m_pEffect->SetTexture("g_txScene", pBoneMesh->vecTexture[pBoneCombos[dwAttrib].AttribId]);

			// set the current number of bones; this tells the effect which shader to use
			m_pEffect->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

			// set the technique we use to draw
			m_pEffect->SetTechnique("Skinning20");

			UINT uiPasses, uiPass;

			// run through each pass and draw
			m_pEffect->Begin(&uiPasses, 0);
			for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			{
				m_pEffect->BeginPass(uiPass);
				pBoneMesh->workMesh->DrawSubset(dwAttrib);
				m_pEffect->EndPass();
			}
			m_pEffect->End();
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		Render((Bone*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)	
	{
		Render((Bone*)pBone->pFrameFirstChild);
		
	}
}


void SkinnedMesh::SetAnimationIndex(int index)
{
	if (!animationController)
		return;
	LPD3DXANIMATIONSET pAnimSet = NULL;
	animationController->GetAnimationSet(index, &pAnimSet);
	animationController->SetTrackAnimationSet(0, pAnimSet);
	SAFE_RELEASE(pAnimSet);

}

void SkinnedMesh::Destroy()
{
	AllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)root, &ah);
	SAFE_DELETE_ARRAY(m_pmWorkingPalette);
	SAFE_RELEASE(m_pEffect);
}


void SkinnedMesh::SetRandomTrackPosition()
{
	animationController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}

void SkinnedMesh::SetTransformMat(LPCSTR name, D3DXMATRIX mat)
{
	/*
	Bone* bone = (Bone*)root;

	if (bone->Name == name)
	{
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;
		boneMesh->pBoneOffsetMatrices = &mat;
	}
	*/

	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	temp = mat;

	if (root->Name == name)
	{
		
	}
	
}

LPD3DXEFFECT SkinnedMesh::LoadEffect(char* szFilename)
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL,                          NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	D3DDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	HRESULT hr;
	if (FAILED(hr = D3DXCreateEffectFromFileA(D3DDevice,
		szFilename,
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer)))
	{
		// if creation fails, and debug information has been returned, output debug info
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer());
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}

/*
void SkinnedMesh::Load(const char * foldername, const char * filename)
{
	std::string	folder(foldername);
	std::string fullPath = foldername + std::string("\\") + filename;

	AllocateHierarchy alloc;
	//getset으로 폴더 이름 넣어주기
	alloc.SetFolder(folder);
	//ASCII
	D3DXLoadMeshHierarchyFromXA(
		fullPath.c_str(),
		D3DXMESH_MANAGED,
		D3DDevice,
		&alloc,
		nullptr,
		&root,
		&aniController
	);

	SetupBoneMatrixPtrs(root);
}

void SkinnedMesh::Update(D3DXMATRIX world)
{
	
	float deltaTime = g_pTimeManager->GetDeltaTime();
	//시간 보정 다 해줌
	aniController->AdvanceTime(deltaTime, nullptr);
	mat = &world;
	//---------------------BLEND--------------------------------------------
	if (passedBlendTime <= blendTime)
	{
		passedBlendTime += deltaTime;
		if (passedBlendTime < blendTime)
		{
			float fWeight = passedBlendTime / blendTime;
			//가중치를 넣어서 계산
			aniController->SetTrackWeight(0, fWeight);
			aniController->SetTrackWeight(1, 1- fWeight);
		}
		else
		{
			//가중치를 넣어서 계산
			aniController->SetTrackWeight(0, 1);
			aniController->SetTrackWeight(1, 0);
			//1번 트랙을 펄스 시킴
			aniController->SetTrackEnable(1, false);
		}
	}
	//----------------------------------------------------------------------
	Update(root, nullptr);
	UpdateSkinnedMesh(root);

}

void SkinnedMesh::Update(LPD3DXFRAME frame, LPD3DXFRAME parent)
{
	Bone* bone = (Bone*)frame;
	//월드 매트릭스 = 로컬 매트릭스 개념(원래 멤버)
	bone->CombinedTransformationMatrix = bone->TransformationMatrix;

	if (parent)
	{
		bone->CombinedTransformationMatrix *= ((Bone*)parent)->CombinedTransformationMatrix;
	}

	if (bone->pFrameSibling)
	{
		Update(bone->pFrameSibling, parent);
	}
	
	if (bone->pFrameFirstChild)
	{
		Update(bone->pFrameFirstChild, bone);
	}
}

void SkinnedMesh::Render(LPD3DXFRAME frame)
{
	if (frame == nullptr)
	{
		frame = root;
	}
	if (frame && frame->pMeshContainer)
	{
		//본인이 커스터마이징한 본이랑 본매쉬를 가져온다.
		Bone* bone = (Bone*)frame;
		BoneMesh* boneMesh = (BoneMesh*)frame->pMeshContainer;
		//월드매트릭스 적용해 주고
		D3DDevice->SetTransform(D3DTS_WORLD, &bone->CombinedTransformationMatrix);
		//본매쉬 안에 있던 material and texture set해주기
		//attribute가 몇개가 있는지 모르기 때문에 꼭 루프돌려주자
		for (size_t i = 0; i < boneMesh->materialTextureList.size(); i++)
		{
			D3DDevice->SetMaterial(&boneMesh->materialTextureList[i]->GetMaterial());
			D3DDevice->SetTexture(0, boneMesh->materialTextureList[i]->GetTexture());
			//그리기
			//boneMesh->orgMesh->DrawSubset(i);
			boneMesh->workMesh->DrawSubset(i);
		}
	}

	//자식이 있다면
	if (frame->pFrameFirstChild)
	{
		Render(frame->pFrameFirstChild);
	}

	//형제?가 있다면
	if (frame->pFrameSibling)
	{
		Render(frame->pFrameSibling);
	}
}

void SkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME frame)
{
	// 각 프레임의 메시 컨테이너에 있는 pskininfo를 이용하여 
	// 영향받는 모든 각 프레임의 매트릭스를 ppbonematrix에 연결 
	// pskininfo->getnumbones()으로 영향 받는 본의 개수를 찾음,
	// pskininfo->getnumbones(i) i번 프레임의 이름을 찾음, 
	// d3dxframefind(루트, 스킨인포 본네임 ),
	// 본메쉬의 피피본메트릭스[i]에다가 찾아서 월드매트릭스를 걸어줘라
	// 자식이 있을 경우 자기 자신을 재기 호출해준당
	// 형제가 있을 경우는 재기 호출을 해준당
	if (frame == nullptr) return;

	Bone* bone = (Bone*)frame;
	if (bone->pMeshContainer)
	{
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;
		if (boneMesh->pSkinInfo)
		{
			LPD3DXSKININFO skinInfo = boneMesh->pSkinInfo;
			DWORD dwNumBones = skinInfo->GetNumBones();
			//++i, i++ 차이
			//++i가 생성자가 한번 덜생겨서 빠르다
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				std::string BoneName = skinInfo->GetBoneName(i);
				Bone* fBone = (Bone*)D3DXFrameFind(root, BoneName.c_str());
				boneMesh->ppBoneMatrixPtrs[i] = &(fBone->CombinedTransformationMatrix);
			}
		}
	}

	if (bone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs(bone->pFrameFirstChild);
	}
	if (bone->pFrameSibling)
	{
		SetupBoneMatrixPtrs(bone->pFrameSibling);
	}
}

void SkinnedMesh::UpdateSkinnedMesh(LPD3DXFRAME frame)
{
	if (frame == nullptr) return;

	Bone* bone = (Bone*)frame;
	if (bone->pMeshContainer)
	{
		//
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;
		if (boneMesh->pSkinInfo)
		{
			LPD3DXSKININFO skinInfo = boneMesh->pSkinInfo;
			DWORD dwNumBones = skinInfo->GetNumBones();

			for (DWORD i = 0; i < dwNumBones; i++)
			{
				if (mat)
				{
					boneMesh->pCurrentBoneMatrices[i] = boneMesh->pBoneOffsetMatrices[i] *
						*(boneMesh->ppBoneMatrixPtrs[i]) * *mat;
				}

				else
				{
					boneMesh->pCurrentBoneMatrices[i] = boneMesh->pBoneOffsetMatrices[i] *
						*(boneMesh->ppBoneMatrixPtrs[i]);
				}
				
			}

			BYTE* src = nullptr;
			BYTE* dest = nullptr;

			//버텍스 버퍼 락 언락 
			boneMesh->orgMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
			boneMesh->workMesh->LockVertexBuffer(0, (void**)&dest);

			boneMesh->pSkinInfo->UpdateSkinnedMesh(boneMesh->pCurrentBoneMatrices, NULL, src, dest);

			boneMesh->workMesh->UnlockVertexBuffer();
			boneMesh->orgMesh->UnlockVertexBuffer();

		}
	}
	if (bone->pFrameFirstChild)
	{
		UpdateSkinnedMesh(bone->pFrameFirstChild);
	}
	if (bone->pFrameSibling)
	{
		UpdateSkinnedMesh(bone->pFrameSibling);
	}
}

void SkinnedMesh::SetAnimationIndex(int index, bool isBlend)
{
	//지나간 시간 초기화!!(중요함)
	passedBlendTime = 0.0f;
	//다음 애니메이션 셋
	LPD3DXANIMATIONSET pNextAnimSet = nullptr;
	aniController->GetAnimationSet(index, &pNextAnimSet);
	//신호 -> 여기 셋애니메이션인덱스 -> 업데이트
	if (isBlend)
	{
		//이전 애니메이션 셋
		LPD3DXANIMATIONSET pPrevAniSet = nullptr;
		//현재 애니메이션 넣어주기
		aniController->GetTrackAnimationSet(0, &pPrevAniSet);
		
		//weight를 설정해서 사용(중량감)
		D3DXTRACK_DESC StTrackDesc;
		//getTrackDesc 0, 1
		//0 -> 0.0f , 1 -> 1,0f
		aniController->GetTrackDesc(0, &StTrackDesc);
		//0번 트랙에서 가져온 트랙데스크를 1번 트랙에다가 넣는다
		aniController->SetTrackDesc(1, &StTrackDesc);

		aniController->SetTrackWeight(0, 0);
		aniController->SetTrackWeight(1, 1);

		aniController->SetTrackAnimationSet(1, pPrevAniSet);
	}

	aniController->SetTrackAnimationSet(0, pNextAnimSet);
	SAFE_RELEASE(pNextAnimSet);
}
*/