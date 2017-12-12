#pragma once

//class CubeCtrl;
class CubeMan;
class CameraCtrl;
class GridCtrl;
class RayCtrl;
class CharacterCtrl;
class LinearInterpolation;
class Group;
class iMap;
class OBJMap;
class ASECharacter;
class HeightMap;
class SkinnedMesh;
class FrustumCulling;
class SkyBox;

struct m4_setting
{
	bool isStock;
	bool isSight;
	bool isMag;
	bool isCompen;
	bool isGrip;
};

class MainGameManager
{
	SINGLETON(MainGameManager)

	void Init ();
	void Update ();
	void Render ( );
	void WndProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void Destroy();

private:

	void SetLight ();
	void m4Setting();
	void wearing();

	CameraCtrl  cameraCtrl;
	GridCtrl    gridCtrl;
	CharacterCtrl characterCtrl;
	FrustumCulling* Culling;
	UIManager	uiManager;
	//LinearInterpolation* lerp;

	std::vector<Group*> groupList;
	std::vector<Group*> groupList2;
	std::vector<MaterialTexture*> mList;
	std::vector<MaterialTexture*> mList2;
	std::vector<SkinnedMesh*> skinList;

	OBJMap*				mapObject;
	CubeCtrl*			cubeCtrl;
	ASECharacter*		woman;
	LPD3DXMESH			mesh;
	LPD3DXMESH			mesh2;
	HeightMap*			map;
	SkinnedMesh*		zombie;
	SkinnedMesh*		m4;
	SkinnedMesh*		akm;
	SkinnedMesh*		pan;
	SkinnedMesh*		coat;
	SkyBox				skyBox;

	m4_setting			m4Set;

	float posX, posY, posZ = 0;
	int indexNum = 0;

};

