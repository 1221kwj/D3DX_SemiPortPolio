#pragma once
#include "gameNode.h"
#include "CharCtrl.h"

class SkinnedMesh;

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

private:
	CharCtrl* control;
	SkinnedMesh* test_Human;
};

