#include "stdafx.h"
#include "SkinnedMeshManager.h"

SkinnedMeshManager::SkinnedMeshManager()
{
}
SkinnedMeshManager::~SkinnedMeshManager()
{
}

SkinnedMesh * SkinnedMeshManager::GetSkinnedMesh(char * szFolder, char * szFile)
{
	std::string fullPath = szFolder + std::string("\\") + szFile;

	if (skinnedMeshMapList.find(fullPath) == skinnedMeshMapList.end())
	{
		SkinnedMesh* mesh = new SkinnedMesh;
		mesh->Load(szFolder, szFile);
		
		skinnedMeshMapList[fullPath] = mesh;
	}

	return skinnedMeshMapList[fullPath];
}

void SkinnedMeshManager::Destroy()
{
	//for each(auto p in skinnedMeshMapList)
	//{
	//	p.second->Destroy();
	//}
}

void SkinnedMeshManager::addChild(LPD3DXFRAME pBone, LPD3DXFRAME pBone2)
{
	pBone->pFrameFirstChild = pBone2;
}

void SkinnedMeshManager::addSibilng(LPD3DXFRAME pBone, LPD3DXFRAME pBone2)
{
	pBone->pFrameSibling = pBone2;
}

void SkinnedMeshManager::change(LPD3DXFRAME pBone, LPD3DXFRAME pBone2)
{
	pBone2->pMeshContainer = pBone->pMeshContainer;
	//pBone->pMeshContainer = pBone2->pMeshContainer;
}
