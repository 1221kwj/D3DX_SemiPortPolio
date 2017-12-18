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
	//	## 각종 선언 초기화 부분
	//============================================
	loading = new Loading;
	loading->init();

	//============================================
	//============================================
	//	## 각 부분 초기화 함수 자리
	//	## 추가 시 이 부분 사용
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
	loading->loadSkinnedMesh("사람", "./zombie/human/", "human.x");
	loading->loadSkinnedMesh("좀비", "./zombie/Lv1/", "zombie1.x");
	loading->loadSkinnedMesh("m4", "./m4", "m4_base.x");
	loading->loadSkinnedMesh("pan", "./weapon", "pan.x");
	loading->loadSkinnedMesh("akm", "./akm", "akm_base.x");
	loading->loadSkinnedMesh("보정기", "./m4/", "m4_compen.x");
	loading->loadSkinnedMesh("앵글손잡이", "./m4/", "m4_grip.x");
	loading->loadSkinnedMesh("개머리판", "./m4/", "m4_stock.x");
	loading->loadSkinnedMesh("기본개머리판", "./m4/", "m4_base_stock.x");
	loading->loadSkinnedMesh("대탄", "./m4/", "m4_lemag.x");
	loading->loadSkinnedMesh("기본탄", "./m4/", "m4_base_mag.x");
	loading->loadSkinnedMesh("4배율", "./m4/", "m4_acog.x");
	loading->loadSkinnedMesh("가늠좌", "./m4/", "m4_base_sight.x");
	
	//NoBoneMesh
	loading->loadNoBoneMesh("터널", "./map/", "tunnel3_2.x");
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

