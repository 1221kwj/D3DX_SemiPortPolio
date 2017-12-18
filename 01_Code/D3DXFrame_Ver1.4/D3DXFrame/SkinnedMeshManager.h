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
	//���� �ִ� ��Ų�� �޽�
	SkinnedMesh*	GetSkinnedMesh( char* skinName );
	SkinnedMesh*	GetSkinnedMesh( char* skinName, char* szFolder, char* szFile );
	//���ٺ� �޽�
	NoBoneMesh*		GetNoBoneMesh(char* Name);
	NoBoneMesh*		GetNoBoneMesh(char* Name, char* szFolder, char* szFile);
};

