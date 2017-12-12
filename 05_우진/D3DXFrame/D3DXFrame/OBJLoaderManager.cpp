#include "stdafx.h"
#include "OBJLoaderManager.h"

OBJLoaderManager:: OBJLoaderManager() {}
OBJLoaderManager::~OBJLoaderManager() {}

void OBJLoaderManager::release()
{
	for each ( auto iter in groupMapList )
	{
		for each ( auto groupIter in iter.second )
		{
			groupIter->Release();
		}
		iter.second.clear();
	}

	groupMapList.clear();
}

void OBJLoaderManager::addOBJFile( char * groupName, char * szFolder, char * szFile )
{
	if ( groupMapList.find( groupName ) == groupMapList.end() )
	{
		string szFullPath( szFolder );
		
		szFullPath += string( szFile );
		
		OBJLoader* tempOBJFile = new OBJLoader;
		
		GROUPLIST  newGroupList;
		
		tempOBJFile->Load( szFolder, szFile, nullptr, newGroupList );
		
		groupMapList[ groupName ] = newGroupList;
	}
	else return;
}

void OBJLoaderManager::delOBJFile( char * groupName )
{
	if ( groupMapList.find( groupName ) == groupMapList.end() )
	{
		WCHAR endMsg[ 128 ];

		wsprintf( endMsg, L"등록 되지 않은 OBJ 파일입니다." );

		if ( MessageBox( _hWnd, endMsg, L"OBJLoaderManager", MB_OK ) == IDOK )
		{
			PostMessage( _hWnd, WM_DESTROY, 0, 0 );
		}
	}

	groupMapList.erase( groupName );
}

GROUPLIST OBJLoaderManager::GetGroup( char* groupName )
{
	if ( groupMapList.find( groupName ) == groupMapList.end() )
	{
		WCHAR endMsg[ 128 ];

		wsprintf( endMsg, L"등록 되지 않은 OBJ 파일입니다." );

		if ( MessageBox( _hWnd, endMsg, L"OBJLoaderManager", MB_OK ) == IDOK )
		{
			PostMessage( _hWnd, WM_DESTROY, 0, 0 );
		}
	}
	return groupMapList[ groupName ];
}

