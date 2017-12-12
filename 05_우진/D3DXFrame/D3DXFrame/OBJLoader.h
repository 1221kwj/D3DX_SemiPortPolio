#pragma once
#include "Object.h"

class MaterialTexture;
class Group;

class OBJLoader : public Object
{
public:
	OBJLoader();
	~OBJLoader();

	void SetPath(char* path);
	void Load(IN char* szFolder, IN char* szFile, IN D3DXMATRIXA16* pmat, OUT std::vector<Group*>& vecGroup);

protected:
	void LoadMtlLib(char* szFullPath);
	
	map<string, MaterialTexture*>  mapMaterialTexture;
	string path;
};

