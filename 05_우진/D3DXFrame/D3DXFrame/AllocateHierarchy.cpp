#include "stdafx.h"

#include "MaterialTexture.h"

#include "AllocateHierarchy.h"


AllocateHierarchy::AllocateHierarchy()
	: folder( "" )
	, defaultPaletteSize( 0 )
	, maxPaletteSize( 0 )
	, minVector( 0, 0, 0 )
	, maxVector( 0, 0, 0 )
{
}

AllocateHierarchy::~AllocateHierarchy() {}

STDMETHODIMP AllocateHierarchy::CreateFrame( LPCSTR Name, LPD3DXFRAME * ppNewFrame )
{
	Bone* frame = new Bone;

	frame->Name = NULL;
	if ( Name )
	{
		frame->Name = new char[ strlen( Name ) + 1 ];
		strcpy_s( frame->Name, strlen( Name ) + 1, Name );
		//strcpy( frame->Name, Name );
	}

	frame->pFrameFirstChild = nullptr;
	frame->pFrameSibling = nullptr;
	frame->pMeshContainer = nullptr;

	D3DXMatrixIdentity( &frame->TransformationMatrix );
	D3DXMatrixIdentity( &frame->CombinedTransformationMatrix );

	*ppNewFrame = frame;

	return S_OK;
}

STDMETHODIMP AllocateHierarchy::CreateMeshContainer(
	LPCSTR						Name,
	CONST D3DXMESHDATA*			pMeshData,
	CONST D3DXMATERIAL*			pMaterials,
	CONST D3DXEFFECTINSTANCE*	pEffectInstances,
	DWORD						NumMaterials,
	CONST DWORD*				pAdjacency,
	LPD3DXSKININFO				pSkinInfo,
	LPD3DXMESHCONTAINER*		ppNewMeshContainer )
{
	assert( pMeshData && pMeshData->Type == D3DXMESHTYPE_MESH );

	// 메쉬 컨테이너 생성
	BoneMesh* pBoneMesh = new BoneMesh;

	// 이름 무시
	pBoneMesh->Name = NULL;


	for ( DWORD i = 0; i < NumMaterials; ++i )
	{
		pBoneMesh->vecMaterial.push_back( pMaterials[ i ].MatD3D );
		
		std::string sFilename( pMaterials[ i ].pTextureFilename );
		
		std::string sFullPath = folder + sFilename;
		
		LPDIRECT3DTEXTURE9 pTex = TEXTUREMANAGER->GetTexture( sFullPath );
		
		pBoneMesh->vecTexture.push_back( pTex );
	}

	pBoneMesh->pEffects = nullptr;

	if ( pAdjacency )
	{
		DWORD dwNumFaces = pMeshData->pMesh->GetNumFaces();
		
		pBoneMesh->pAdjacency = new DWORD[ 3 * dwNumFaces ];
		
		memcpy( pBoneMesh->pAdjacency, pAdjacency, 3 * sizeof( DWORD ) * dwNumFaces );
	}

	SAFE_ADDREF( pSkinInfo );
	pBoneMesh->pSkinInfo = pSkinInfo;

	if ( pMeshData && pMeshData->pMesh )
	{
		D3DXVECTOR3 vMin( 0, 0, 0 ), vMax( 0, 0, 0 );
		
		LPVOID pV = NULL;

		pMeshData->pMesh->LockVertexBuffer( 0, &pV );
		D3DXComputeBoundingBox( 
			( D3DXVECTOR3* )pV,
			pMeshData->pMesh->GetNumVertices(),
			D3DXGetFVFVertexSize( pMeshData->pMesh->GetFVF() ),
			&vMin,
			&vMax );
		D3DXVec3Minimize( &minVector, &minVector, &vMin );
		D3DXVec3Maximize( &maxVector, &maxVector, &vMax );
		pMeshData->pMesh->UnlockVertexBuffer();

		pMeshData->pMesh->CloneMeshFVF(
			pMeshData->pMesh->GetOptions(),
			pMeshData->pMesh->GetFVF(),
			DEVICE,
			&pBoneMesh->orgMesh );
	}

	if ( pSkinInfo ) 
	{
		DWORD dwNumBones = pSkinInfo->GetNumBones();
		pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[ dwNumBones ];
		pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[ dwNumBones ];
		
		for ( DWORD i = 0; i < dwNumBones; ++i )
		{
			pBoneMesh->ppBoneMatrixPtrs[ i ] = NULL;
			
			pBoneMesh->pBoneOffsetMatrices[ i ] = *( pSkinInfo->GetBoneOffsetMatrix( i ) );
		}

		pBoneMesh->dwNumPaletteEntries = min( defaultPaletteSize, pSkinInfo->GetNumBones() );

		if ( maxPaletteSize < pBoneMesh->dwNumPaletteEntries )
		{
			maxPaletteSize = pBoneMesh->dwNumPaletteEntries;
		}

		pBoneMesh->pSkinInfo->ConvertToIndexedBlendedMesh(
			pBoneMesh->orgMesh,
			D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
			pBoneMesh->dwNumPaletteEntries,
			pBoneMesh->pAdjacency,
			NULL,
			NULL,
			NULL,
			&pBoneMesh->dwMaxNumFaceInfls,
			&pBoneMesh->dwNumAttrGroups,
			&pBoneMesh->pBufBoneCombos,
			&pBoneMesh->workMesh );

		DWORD dwOldFVF = pBoneMesh->workMesh->GetFVF();
		DWORD dwNewFVF =
			( dwOldFVF & D3DFVF_POSITION_MASK ) |
			D3DFVF_NORMAL |
			D3DFVF_TEX1 |
			D3DFVF_LASTBETA_UBYTE4;

		if ( dwNewFVF != dwOldFVF )
		{
			LPD3DXMESH pMesh = NULL;

			pBoneMesh->workMesh->CloneMeshFVF(
				pBoneMesh->workMesh->GetOptions(),
				dwNewFVF,
				DEVICE,
				&pMesh );

			SAFE_RELEASE( pBoneMesh->workMesh );
			pBoneMesh->workMesh = pMesh;

			if ( !( dwOldFVF & D3DFVF_NORMAL ) )
			{
				D3DXComputeNormals( pBoneMesh->workMesh, NULL );
			}
		}

		D3DVERTEXELEMENT9 pDecl[ MAX_FVF_DECL_SIZE ];
		D3DVERTEXELEMENT9* pDeclCur;
		pBoneMesh->workMesh->GetDeclaration( pDecl );

		pDeclCur = pDecl;
		while ( pDeclCur->Stream != 0xff )
		{
			if ( ( pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES ) &&
				 ( pDeclCur->UsageIndex == 0 ) )
			{
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			}
				
			pDeclCur++;
		}
		pBoneMesh->workMesh->UpdateSemantics( pDecl );
	}

	*ppNewMeshContainer = pBoneMesh;

	return S_OK;

}

STDMETHODIMP AllocateHierarchy::DestroyFrame( LPD3DXFRAME pFrameToFree )
{
	Bone* bone = ( Bone* )pFrameToFree;

	SAFE_DELETE_ARRAY( bone->Name );
	SAFE_DELETE( bone );

	return S_OK;
}

STDMETHODIMP AllocateHierarchy::DestroyMeshContainer( LPD3DXMESHCONTAINER pMeshContainerToFree )
{
	BoneMesh* boneMesh = ( BoneMesh* )pMeshContainerToFree;

	SAFE_RELEASE( boneMesh->pSkinInfo );
	SAFE_RELEASE( boneMesh->workMesh );
	SAFE_RELEASE( boneMesh->orgMesh );
	SAFE_RELEASE( boneMesh->pBufBoneCombos );

	SAFE_DELETE_ARRAY( boneMesh->Name );
	SAFE_DELETE_ARRAY( boneMesh->pAdjacency );
	SAFE_DELETE_ARRAY( boneMesh->ppBoneMatrixPtrs );
	SAFE_DELETE_ARRAY( boneMesh->pBoneOffsetMatrices );

	SAFE_DELETE( pMeshContainerToFree );

	return S_OK;
}