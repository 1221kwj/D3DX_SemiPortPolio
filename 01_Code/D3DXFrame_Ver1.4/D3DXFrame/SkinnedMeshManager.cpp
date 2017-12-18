#include "stdafx.h"
#include "SkinnedMeshManager.h"

SkinnedMeshManager:: SkinnedMeshManager() {}
SkinnedMeshManager::~SkinnedMeshManager() {}

SkinnedMesh* SkinnedMeshManager::GetSkinnedMesh( char* skinName )
{
	if ( skinnedMeshMapList.find( skinName ) == skinnedMeshMapList.end() )
	{
		WCHAR endMsg[ 128 ];

		wsprintf( endMsg, L"��� ���� ���� X �����Դϴ�." );

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

NoBoneMesh * SkinnedMeshManager::GetNoBoneMesh(char * Name)
{
	if (nbMeshMapList.find(Name) == nbMeshMapList.end())
	{
		WCHAR endMsg[128];

		wsprintf(endMsg, L"��� ���� ���� X �����Դϴ�.");

		if (MessageBox(_hWnd, endMsg, L"XFileManager", MB_OK) == IDOK)
		{
			PostMessage(_hWnd, WM_DESTROY, 0, 0);
		}
	}

	return nbMeshMapList[Name];
}

NoBoneMesh * SkinnedMeshManager::GetNoBoneMesh(char * Name, char * szFolder, char * szFile)
{
	string sFullPath(szFolder);

	sFullPath += string(szFile);

	if (nbMeshMapList.find(Name) == nbMeshMapList.end())
	{
		NoBoneMesh* pNoBoneMesh = new NoBoneMesh;

		pNoBoneMesh->Load(szFolder, szFile);

		nbMeshMapList[Name] = pNoBoneMesh;
	}

	return nbMeshMapList[Name];
}


void SkinnedMeshManager::release()
{
	for each( auto it in skinnedMeshMapList )
	{
		it.second->Destroy();
	}
	
	skinnedMeshMapList.clear();
}