#pragma once
#include "gameNode.h"
enum FILE_PROJECT
{
	SKINNEDMESH_FILE,
	NOBONEMESH_FILE,
	TEXTURE_FILE,
	SOUND_FILE
};

class Loading : public gameNode
{
private:
	vector<SkinnedMesh*>	arrSkinnedMesh;
	vector<NoBoneMesh*>		arrNoBoneMesh;

	UIObject*				obj = nullptr;
	LPD3DXSPRITE			sprite = nullptr;
	FILE_PROJECT			file;
	int						currentGuage;

public:
	Loading();
	~Loading();

	HRESULT init();
	void release();
	void update();
	void render();

	void loadSkinnedMesh(char* skinName, char* szFolder, char* szFile);
	void loadNoBoneMesh(char* skinName, char* szFolder, char* szFile);

	BOOL loadingDone();

	vector<SkinnedMesh*> getLoadSkinnedMesh() { return arrSkinnedMesh; }
	vector<NoBoneMesh*> getLoadNoBoneMesh() { return arrNoBoneMesh; }
};
