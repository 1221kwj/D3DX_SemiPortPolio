#include "stdafx.h"
#include "SkinnedMeshManager.h"

SkinnedMeshManager:: SkinnedMeshManager() {}
SkinnedMeshManager::~SkinnedMeshManager() {}

SkinnedMesh* SkinnedMeshManager::GetSkinnedMesh( char* skinName )
{
	if ( skinnedMeshMapList.find( skinName ) == skinnedMeshMapList.end() )
	{
		WCHAR endMsg[ 128 ];

		wsprintf( endMsg, L"등록 되지 않은 X 파일입니다." );

		if ( MessageBox( _hWnd, endMsg, L"XFileManager", MB_OK ) == IDOK )
		{
			PostMessage( _hWnd, WM_DESTROY, 0, 0 );
		}
	}

	return skinnedMeshMapList[ skinName ];
}

SkinnedMesh* SkinnedMeshManager::GetSkinnedMesh( char* skinName, char* szFolder, char* szFile )
{
	string sFullPath( szFolder );
	
	sFullPath += string( szFile );

	if ( skinnedMeshMapList.find( skinName ) == skinnedMeshMapList.end() )
	{
		SkinnedMesh* pSkinnedMesh = new SkinnedMesh;

		pSkinnedMesh->Load( szFolder, szFile );
		
		skinnedMeshMapList[ skinName ] = pSkinnedMesh;
	}

	return skinnedMeshMapList[ skinName ];
}

void SkinnedMeshManager::release()
{
	for each( auto it in skinnedMeshMapList )
	{
		it.second->Destroy();
	}
	
	skinnedMeshMapList.clear();
}