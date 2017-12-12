
#pragma once
#include "SkinnedMesh.h"

#define g_pSkinnedMeshManager SkinnedMeshManager::GetInstance()

class SkinnedMeshManager
{
	SINGLETON ( SkinnedMeshManager );

public:
	SkinnedMesh* GetSkinnedMesh ( char* szFolder, char* szFile );
	void Destroy ();
	void addChild(LPD3DXFRAME, LPD3DXFRAME);
	void addSibilng(LPD3DXFRAME, LPD3DXFRAME);
	void change(LPD3DXFRAME, LPD3DXFRAME);

private:
	std::map<std::string, SkinnedMesh*> skinnedMeshMapList;

};

