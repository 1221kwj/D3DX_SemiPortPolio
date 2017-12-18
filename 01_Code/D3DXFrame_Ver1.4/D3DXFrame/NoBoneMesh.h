#pragma once
class NoBoneMesh
{
	friend class SkinnedMeshManager;
private:
	ID3DXMesh*					mesh = 0;
	vector<D3DMATERIAL9>		Mtrls;
	vector<IDirect3DTexture9*>	Textures;
	D3DXVECTOR3					positionVec;
	string						fullName;

public:
	NoBoneMesh();
	~NoBoneMesh();

	void Load(char * szFolder, char * szFile);
	void Render();
	void Destroy();
	void SetPosition(D3DXVECTOR3 psV) { positionVec = psV; }

	string GetName() { return fullName; }
};

