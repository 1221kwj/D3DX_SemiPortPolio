#include "stdafx.h"
#include "IntroLoadingScene.h"

IntroLoadingScene::IntroLoadingScene()
{
}


IntroLoadingScene::~IntroLoadingScene()
{
}

HRESULT IntroLoadingScene::init()
{
	//============================================
	//	## ���� ���� �ʱ�ȭ �κ�
	//============================================
	loading = new Loading;
	loading->init();

	//============================================
	//============================================
	//	## �� �κ� �ʱ�ȭ �Լ� �ڸ�
	//	## �߰� �� �� �κ� ���
	XFileSetting();
	OBJFileSetting();
	ControllerSetting();
	UISetting();
	//============================================
	return S_OK;
}

void IntroLoadingScene::XFileSetting()
{
	//SkinnedMesh
	loading->loadSkinnedMesh("���", "./zombie/human/", "human.x");
	loading->loadSkinnedMesh("����", "./zombie/Lv1/", "zombie1.x");
	loading->loadSkinnedMesh("m4", "./m4", "m4_base.x");
	loading->loadSkinnedMesh("pan", "./weapon", "pan.x");
	loading->loadSkinnedMesh("akm", "./akm", "akm_base.x");
	loading->loadSkinnedMesh("������", "./m4/", "m4_compen.x");
	loading->loadSkinnedMesh("�ޱۼ�����", "./m4/", "m4_grip.x");
	loading->loadSkinnedMesh("���Ӹ���", "./m4/", "m4_stock.x");
	loading->loadSkinnedMesh("�⺻���Ӹ���", "./m4/", "m4_base_stock.x");
	loading->loadSkinnedMesh("��ź", "./m4/", "m4_lemag.x");
	loading->loadSkinnedMesh("�⺻ź", "./m4/", "m4_base_mag.x");
	loading->loadSkinnedMesh("4����", "./m4/", "m4_acog.x");
	loading->loadSkinnedMesh("������", "./m4/", "m4_base_sight.x");
	
	//NoBoneMesh
	loading->loadNoBoneMesh("�ͳ�", "./map/", "tunnel3_2.x");
}

void IntroLoadingScene::OBJFileSetting()
{


}

void IntroLoadingScene::ControllerSetting()
{


}

void IntroLoadingScene::UISetting()
{


}

void IntroLoadingScene::release()
{
	SAFE_DELETE(loading);

}

void IntroLoadingScene::update()
{
	loading->update();

	if (loading->loadingDone())
	{
		SCENEMANAGER->changeScene("test");
	}

}

void IntroLoadingScene::render()
{
	loading->render();

}

