#pragma once
#include "gameNode.h"
#include "CharCtrl.h"

class SkinnedMesh;

//�ӽ� ���� m4 �׽�Ʈ��
struct m4_setting
{
	bool isStock;
	bool isSight;
	bool isMag;
	bool isCompen;
	bool isGrip;
};

class testScene : public gameNode
{
public:
	testScene();
	~testScene();

	HRESULT init();
	void	release();
	void	update();
	void	render();
	void	cameraSetting();

	void	charKeySet();
	void	m4Setting();

private:
	CharCtrl*			control;
	SkinnedMesh*		test_Human;
	SkinnedMesh*		m4;
	SkinnedMesh*		akm;
	SkinnedMesh*		pan;
	m4_setting			m4Set;
	enemy*				zombie1;
};

