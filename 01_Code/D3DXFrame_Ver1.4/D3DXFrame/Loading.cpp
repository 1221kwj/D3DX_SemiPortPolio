#include "stdafx.h"
#include "Loading.h"


Loading::Loading()
{
}


Loading::~Loading()
{
}

HRESULT Loading::init()
{
	D3DXIMAGE_INFO imageIfo;
	LPDIRECT3DTEXTURE9 pTexture = TEXTUREMANAGER->GetTexture("./Background/LoadingScreen.tga", &imageIfo);

	UIImageView* imageView = new UIImageView;
	imageView->SetTexture(pTexture);
	imageView->SetSize(D3DXVECTOR2(imageIfo.Width, imageIfo.Height));
	imageView->SetLocalPos(D3DXVECTOR3(-340, -200, 0));

	obj = imageView;

	currentGuage = 0;

	D3DXCreateSprite(DEVICE, &sprite);

	return S_OK;
}

void Loading::release()
{
	SAFE_RELEASE(sprite);
}

void Loading::update()
{
	if (obj) obj->update();
}

void Loading::render()
{
	if (obj) obj->render(sprite);

	if (!loadingDone() && currentGuage < arrSkinnedMesh.size())
	{
		char str[256];
		string frameRate;
		RECT RC_COORD;
		SetRect(&RC_COORD, WINSIZEX / 2 + 300, 0, WINSIZEX + 200, 600);

		LPD3DXFONT font = FONTMANAGER->GetFont(fontManager::NORMAL);

		strcpy_s(str, arrSkinnedMesh[currentGuage]->GetName().c_str());

		font->DrawTextA(nullptr, str, 256, &RC_COORD, DT_LEFT | DT_BOTTOM | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	}
	else if (!loadingDone() && currentGuage > arrSkinnedMesh.size())
	{
		char str[256];
		string frameRate;
		RECT RC_COORD;
		SetRect(&RC_COORD, WINSIZEX/2 + 300 , 0, WINSIZEX + 200, 600);

		LPD3DXFONT font = FONTMANAGER->GetFont(fontManager::NORMAL);

		strcpy_s(str, arrNoBoneMesh[currentGuage - arrSkinnedMesh.size() - 2]->GetName().c_str());

		font->DrawTextA(nullptr, str, 256, &RC_COORD, DT_LEFT | DT_BOTTOM | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	}
}

void Loading::loadSkinnedMesh(char * skinName, char * szFolder, char * szFile)
{
	SKINNEDMESHMANAGER->GetSkinnedMesh(skinName, szFolder, szFile);

	arrSkinnedMesh.push_back(SKINNEDMESHMANAGER->GetSkinnedMesh(skinName));
}

void Loading::loadNoBoneMesh(char * skinName, char * szFolder, char * szFile)
{
	SKINNEDMESHMANAGER->GetNoBoneMesh(skinName, szFolder, szFile);

	arrNoBoneMesh.push_back(SKINNEDMESHMANAGER->GetNoBoneMesh(skinName));
}

BOOL Loading::loadingDone()
{
	if (currentGuage > arrSkinnedMesh.size() + arrNoBoneMesh.size())
	{
		return true;
	}
	//트루가 되지 않으면 카운트가 쌓임
	currentGuage++;

	return false;
}