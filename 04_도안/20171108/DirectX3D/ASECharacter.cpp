#include "stdafx.h"
#include "ASECharacter.h"
#include "ASEFrame.h"
#include "ASELoader.h"

ASECharacter::ASECharacter()
	:root(NULL)
{
}

ASECharacter::~ASECharacter()
{
	root->Destroy();	
}

void ASECharacter::Load(char * filename)
{
	ASELoader* nFileLoader = new ASELoader;
	root = new ASEFrame;
	root = nFileLoader->Load(filename,&aseScene);
	SAFE_RELEASE(nFileLoader);
}

void ASECharacter::Update(D3DXMATRIXA16* parent)
{
	
	int firstTick = aseScene.firstFrame * aseScene.tickPerFrame;
	int lastTick = aseScene.lastFrame * aseScene.tickPerFrame;
	int tickSpeed = aseScene.frameSpeed * aseScene.tickPerFrame;
	//int testNum = (int)((tickSpeed / 1000.f)*GetTickCount());
	int keyFrame = (int)((tickSpeed/1000.f)*GetTickCount())%(lastTick - firstTick) + firstTick;

	if(root)root->Update(keyFrame, parent);
	
}

void ASECharacter::Render()
{
	root->Render();
}
