#pragma once
#include "singletonBase.h"
#include "SkinnedMesh.h"
#include "NoBoneMesh.h"

class SkinnedMeshManager : public singletonBase <SkinnedMeshManager>
{
private:
	map<string, SkinnedMesh*>	skinnedMeshMapList;
	map<string, NoBoneMesh*>	nbMeshMapList;

public:
	SkinnedMeshManager();
	~SkinnedMeshManager();

	void release();
	//뼈가 있는 스킨드 메시
	SkinnedMesh*	GetSkinnedMesh( char* skinName );
	SkinnedMesh*	GetSkinnedMesh( char* skinName, char* szFolder, char* szFile );
	//무근본 메시
	NoBoneMesh*		GetNoBoneMesh(char* Name);
	NoBoneMesh*		GetNoBoneMesh(char* Name, char* szFolder, char* szFile);
};

