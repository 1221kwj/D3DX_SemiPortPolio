#include "stdafx.h"

#include "AllocateHierarchy.h"
#include "MaterialTexture.h"

#include "SkinnedMesh.h"

SkinnedMesh::SkinnedMesh()
	: root					( nullptr )
	, animationController	( nullptr )
	//, blendTime (0.3f)
	//, passedBlendTime(0.0f)
	, m_dwWorkingPaletteSize( 0 )
	, m_pmWorkingPalette	( NULL )
	, m_pEffect				( NULL )
	, m_vPosition			( 0, 0, 0 )
{
}

SkinnedMesh::SkinnedMesh( char* skinName, char* szFolder, char* szFilename )
	: root					( NULL )
	, animationController	( NULL )
	, m_dwWorkingPaletteSize( 0 )
	, m_pmWorkingPalette	( NULL )
	, m_pEffect				( NULL )
	, m_vPosition			( 0, 0, 0 )
{
	SkinnedMesh* skinnedMesh = SKINNEDMESHMANAGER->GetSkinnedMesh( skinName, szFolder, szFilename );

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
		&animationController );

}

SkinnedMesh::~SkinnedMesh() {}

void SkinnedMesh::release()
{
	SAFE_RELEASE( animationController );
}

void SkinnedMesh::Load( const char * foldername, const char * filename )
{
	m_pEffect = LoadEffect( "MultiAnimation.hpp" );

	int nPaletteSize = 0;
	m_pEffect->GetInt( "MATRIX_PALETTE_SIZE", &nPaletteSize );

	std::string folder( foldername );
	std::string fullpath = foldername + std::string( "\\" ) + filename;

	fullName = fullpath;

	AllocateHierarchy alloc;
	alloc.SetFolder( folder );
	alloc.SetDefaultPaletteSize( nPaletteSize );

	m_stBoundingSphere.vCenter = ( alloc.GetMinVector() + alloc.GetMaxVector() ) / 2.0f;
	m_stBoundingSphere.fRadius = D3DXVec3Length( &( alloc.GetMinVector() - alloc.GetMaxVector() ) );

	D3DXLoadMeshHierarchyFromXA(
		fullpath.c_str(),
		D3DXMESH_MANAGED,
		DEVICE,
		&alloc,
		nullptr,
		( LPD3DXFRAME* )&root,
		&animationController
	);

	if ( m_pmWorkingPalette )
		delete[] m_pmWorkingPalette;

	m_dwWorkingPaletteSize = alloc.GetMaxPaletteSize();
	m_pmWorkingPalette = new D3DXMATRIX[ m_dwWorkingPaletteSize ];
	if ( m_pmWorkingPalette == NULL )
	{
		m_dwWorkingPaletteSize = 0;
	}

	if ( root ) SetupBoneMatrixPtrs( root );

	D3DXMATRIX worldTM;
	D3DXMatrixIdentity( &worldTM );
	
	Update( root, &worldTM );
}

void SkinnedMesh::Update( Bone* frame, D3DXMATRIX* pmatParent )
{
	frame->CombinedTransformationMatrix = frame->TransformationMatrix;
	if ( pmatParent )
	{
		frame->CombinedTransformationMatrix =
			frame->CombinedTransformationMatrix * ( *pmatParent );
	}

	if ( frame->pFrameSibling )
	{
		Update( ( Bone* )frame->pFrameSibling, pmatParent );
	}

	if ( frame->pFrameFirstChild )
	{
		Update( ( Bone* )frame->pFrameFirstChild, &( frame->CombinedTransformationMatrix ) );
	}
}


void SkinnedMesh::SetupBoneMatrixPtrs( LPD3DXFRAME frame )
{
	if ( frame == nullptr ) return;
	
	Bone* bone = ( Bone* )frame;
	if ( bone->pMeshContainer )
	{
		BoneMesh* boneMesh = ( BoneMesh* )bone->pMeshContainer;
		if ( boneMesh->pSkinInfo )
		{
			LPD3DXSKININFO skinInfo = boneMesh->pSkinInfo;
			DWORD dwNumBones = boneMesh->pSkinInfo->GetNumBones();
			for ( DWORD i = 0; i < dwNumBones; ++i )
			{
				LPCSTR szBoneName = skinInfo->GetBoneName( i );
				if ( szBoneName == NULL || strlen( szBoneName ) == 0 )
					continue;

				Bone* p = ( Bone* )D3DXFrameFind( root, skinInfo->GetBoneName( i ) );
				boneMesh->ppBoneMatrixPtrs[ i ] = &( p->CombinedTransformationMatrix );
			}
		}

	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if ( frame->pFrameFirstChild )
	{
		SetupBoneMatrixPtrs( frame->pFrameFirstChild );
	}

	if ( frame->pFrameSibling )
	{
		SetupBoneMatrixPtrs( frame->pFrameSibling );
	}
}

//void SkinnedMesh::UpdateSkinnedMesh ( LPD3DXFRAME frame)
//{
//	// pCurrentBoneMatrices 를 계산하시오
//	// pCurrentBoneMatrices = pBoneOffsetMatrices * ppBoneMatrixPtrs 
//
//	if ( frame == nullptr )
//		return;
//
//	Bone* bone = ( Bone* ) frame;
//	if ( bone->pMeshContainer )
//	{
//		BoneMesh* boneMesh = ( BoneMesh* ) bone->pMeshContainer;
//		if ( boneMesh->pSkinInfo )
//		{
//			LPD3DXSKININFO pSkinInfo = boneMesh->pSkinInfo;
//			DWORD dwNumBones = pSkinInfo->GetNumBones ();
//			for ( DWORD i = 0; i < dwNumBones; ++i )
//			{
//				boneMesh->pCurrentBoneMatrices[i] =
//					boneMesh->pBoneOffsetMatrices[i] *
//					*( boneMesh->ppBoneMatrixPtrs[i] );
//			}
//
//			BYTE* src = nullptr;
//			BYTE* dest = nullptr;
//
//			boneMesh->orgMesh->LockVertexBuffer ( D3DLOCK_READONLY, ( void** ) &src );
//			boneMesh->workMesh->LockVertexBuffer ( 0, ( void** ) &dest );
//
//			//MeshData.pMesh을 업데이트 시켜준다.
//			pSkinInfo->UpdateSkinnedMesh (
//				boneMesh->pCurrentBoneMatrices, nullptr, src, dest );
//
//			boneMesh->workMesh->UnlockVertexBuffer ();
//			boneMesh->orgMesh->UnlockVertexBuffer ();
//		}
//	}
//
//	//재귀적으로 모든 프레임에 대해서 실행.
//	if ( frame->pFrameFirstChild )
//	{
//		UpdateSkinnedMesh ( frame->pFrameFirstChild );
//	}
//
//	if ( frame->pFrameSibling )
//	{
//		UpdateSkinnedMesh ( frame->pFrameSibling );
//	}
//
//}

void SkinnedMesh::UpdateAndRender()
{
	float deltaTime = TIMEMANAGER->getElapsedTime();

	if ( animationController )
	{
		animationController->AdvanceTime(deltaTime, NULL );
	}
	//---------------------BLEND--------------------------------------------
	if (passedBlendTime <= blendTime)
	{
		passedBlendTime += deltaTime;
		if (passedBlendTime < blendTime)
		{
			float fWeight = passedBlendTime / blendTime;
			//가중치를 넣어서 계산
			animationController->SetTrackWeight(0, fWeight);
			animationController->SetTrackWeight(1, 1 - fWeight);
		}
		else
		{
			//가중치를 넣어서 계산
			animationController->SetTrackWeight(0, 1);
			animationController->SetTrackWeight(1, 0);
			//1번 트랙을 펄스 시킴
			animationController->SetTrackEnable(1, false);
		}
	}
	//----------------------------------------------------------------------
	if ( root )
	{
		D3DXMATRIX matTrans, matRot, worldTM;
		D3DXMatrixTranslation( &matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z );
		D3DXMatrixRotationAxis( &matRot, &D3DXVECTOR3(0, 1, 0), angle );

		worldTM = matRot * matTrans;
		//로테이트 메트릭스를 가져와서 요긴하게 씁니다..
		rotateMat = matRot;
		Update( root, &worldTM );
		Render( root );
	}

}

void SkinnedMesh::Render( Bone* pBone )
{
	assert( pBone );

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if ( pBone->pMeshContainer )
	{
		BoneMesh* pBoneMesh = ( BoneMesh* )pBone->pMeshContainer;

		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos =
			( LPD3DXBONECOMBINATION )( pBoneMesh->pBufBoneCombos->GetBufferPointer() );

		D3DXMATRIXA16 matViewProj, matView, matProj;
		DEVICE->GetTransform( D3DTS_VIEW, &matView );
		DEVICE->GetTransform( D3DTS_PROJECTION, &matProj );
		matViewProj = matView * matProj;

		D3DXMATRIXA16 mView, mInvView;
		DEVICE->GetTransform( D3DTS_VIEW, &mView );
		D3DXMatrixInverse( &mInvView, 0, &mView );
		D3DXVECTOR3 vEye = D3DXVECTOR3( 0, 0, 0 );
		D3DXVec3TransformCoord( &vEye, &vEye, &mInvView );

		D3DXMATRIXA16 scaleTM;
		D3DXMatrixIdentity( &scaleTM );
		D3DXMatrixScaling( &scaleTM, 0.1, 0.1, 0.1 );
		//DEVICE->SetTransform( D3DTS_WORLD, &scaleTM );

		// for each palette
		for ( DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib )
		{
			// set each transform into the palette
			for ( DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry )
			{
				DWORD dwMatrixIndex = pBoneCombos[ dwAttrib ].BoneId[ dwPalEntry ];
				if ( dwMatrixIndex != UINT_MAX )
				{
					m_pmWorkingPalette[ dwPalEntry ] =
						pBoneMesh->pBoneOffsetMatrices[ dwMatrixIndex ] *
						( *pBoneMesh->ppBoneMatrixPtrs[ dwMatrixIndex ] );// *worldTM;// *scaleTM
				}
			}

			// set the matrix palette into the effect
			m_pEffect->SetMatrixArray( "amPalette",
				m_pmWorkingPalette,
				pBoneMesh->dwNumPaletteEntries );

			m_pEffect->SetMatrix( "g_mViewProj", &matViewProj );
			m_pEffect->SetVector( "vLightDiffuse", &D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ) );
			m_pEffect->SetVector( "vWorldLightPos", &D3DXVECTOR4( 500.0f, 500.0f, 500.0f, 1.0f ) );
			m_pEffect->SetVector( "vWorldCameraPos", &D3DXVECTOR4( vEye, 1.0f ) );
			m_pEffect->SetVector( "vMaterialAmbient", &D3DXVECTOR4( 0.53f, 0.53f, 0.53f, 0.53f ) );
			m_pEffect->SetVector( "vMaterialDiffuse", &D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ) );

			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here
			m_pEffect->SetTexture( "g_txScene", pBoneMesh->vecTexture[ pBoneCombos[ dwAttrib ].AttribId ] );

			// set the current number of bones; this tells the effect which shader to use
			m_pEffect->SetInt( "CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1 );

			// set the technique we use to draw
			m_pEffect->SetTechnique( "Skinning20" );

			UINT uiPasses, uiPass;

			// run through each pass and draw
			m_pEffect->Begin( &uiPasses, 0 );
			for ( uiPass = 0; uiPass < uiPasses; ++uiPass )
			{
				m_pEffect->BeginPass( uiPass );
				pBoneMesh->workMesh->DrawSubset( dwAttrib );
				m_pEffect->EndPass();
			}
			m_pEffect->End();
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if ( pBone->pFrameSibling )
	{
		Render( ( Bone* )pBone->pFrameSibling );
	}

	if ( pBone->pFrameFirstChild )
	{
		Render( ( Bone* )pBone->pFrameFirstChild );
	}
}


void SkinnedMesh::SetAnimationIndex( int index )
{
	if ( !animationController )
		return;
	LPD3DXANIMATIONSET pAnimSet = NULL;
	animationController->GetAnimationSet( index, &pAnimSet );
	animationController->SetTrackAnimationSet( 0, pAnimSet );
	SAFE_RELEASE( pAnimSet );

}
//블렌딩 전용 
void SkinnedMesh::SetAnimationIndex(int index, bool isBlend)
{
	//지나간 시간 초기화!!(중요함)
	passedBlendTime = 0.0f;
	//다음 애니메이션 셋
	LPD3DXANIMATIONSET pNextAnimSet = nullptr;
	animationController->GetAnimationSet(index, &pNextAnimSet);
	//신호 -> 여기 셋애니메이션인덱스 -> 업데이트
	if (isBlend)
	{
		//이전 애니메이션 셋
		LPD3DXANIMATIONSET pPrevAniSet = nullptr;
		//현재 애니메이션 넣어주기
		animationController->GetTrackAnimationSet(0, &pPrevAniSet);

		//weight를 설정해서 사용(중량감)
		D3DXTRACK_DESC StTrackDesc;
		//getTrackDesc 0, 1
		//0 -> 0.0f , 1 -> 1,0f
		animationController->GetTrackDesc(0, &StTrackDesc);
		//0번 트랙에서 가져온 트랙데스크를 1번 트랙에다가 넣는다
		animationController->SetTrackDesc(1, &StTrackDesc);

		animationController->SetTrackWeight(0, 0);
		animationController->SetTrackWeight(1, 1);

		animationController->SetTrackAnimationSet(1, pPrevAniSet);
	}

	animationController->SetTrackAnimationSet(0, pNextAnimSet);
	SAFE_RELEASE(pNextAnimSet);
}

void SkinnedMesh::Destroy()
{
	AllocateHierarchy ah;
	D3DXFrameDestroy( ( LPD3DXFRAME )root, &ah );
	SAFE_DELETE_ARRAY( m_pmWorkingPalette );
	SAFE_RELEASE( m_pEffect );
}


void SkinnedMesh::SetRandomTrackPosition()
{
	animationController->SetTrackPosition( 0, ( rand() % 100 ) / 10.0f );
}



LPD3DXEFFECT SkinnedMesh::LoadEffect( char* szFilename )
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[ 2 ] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL,                          NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	DEVICE->GetDeviceCaps( &caps );
	if ( caps.VertexShaderVersion > D3DVS_VERSION( 1, 1 ) )
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
	if ( FAILED( hr = D3DXCreateEffectFromFileA(
		DEVICE,
		szFilename,
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer ) ) )
	{
		// if creation fails, and debug information has been returned, output debug info
		if ( pBuffer )
		{
			OutputDebugStringA( ( char* )pBuffer->GetBufferPointer() );
			SAFE_RELEASE( pBuffer );
		}

		return NULL;
	}

	return pEffect;
}

LPD3DXFRAME SkinnedMesh::GetRoot()
{
	return root;
}

D3DXMATRIX SkinnedMesh::GetCombineMatrix( char * frameName )
{
	LPD3DXFRAME findFrame = D3DXFrameFind(root, frameName);
	Bone* findBone = ( Bone* )findFrame;
	return findBone->CombinedTransformationMatrix;
}