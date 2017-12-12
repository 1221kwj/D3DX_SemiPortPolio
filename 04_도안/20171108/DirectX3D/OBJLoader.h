
#pragma once


class MaterialTexture;
class Group;

class OBJLoader : public Object
{
public:
	OBJLoader ();
	~OBJLoader ();
	

	void SetPath (char* path);
	void Load ( IN char* szFullPath,
		IN D3DXMATRIXA16* pmat,
		OUT std::vector<Group*>& vecGroup );
	LPD3DXMESH LoadMesh(IN char* szFullPath,
		IN D3DXMATRIXA16* pmat,
		OUT std::vector<MaterialTexture*>& mtList);
	

protected:
	void LoadMtlLib ( char* szFullPath );

protected:
	std::map<std::string, MaterialTexture*>  mapMaterialTexture;
	std::string path;


};

