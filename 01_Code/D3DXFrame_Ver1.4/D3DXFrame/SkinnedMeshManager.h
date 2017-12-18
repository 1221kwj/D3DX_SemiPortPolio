#pragma once
#include "singletonBase.h"
#include "SkinnedMesh.h"

class SkinnedMeshManager : public singletonBase <SkinnedMeshManager>
{
private:
	map<string, SkinnedMesh*> skinnedMeshMapList;

public:
	SkinnedMeshManager();
	~SkinnedMeshManager();

	void release();
	SkinnedMesh* GetSkinnedMesh( char* skinName );
	SkinnedMesh* GetSkinnedMesh( char* skinName, char* szFolder, char* szFile );
};

