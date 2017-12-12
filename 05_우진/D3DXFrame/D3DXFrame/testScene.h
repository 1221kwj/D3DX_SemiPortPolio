#pragma once
#include "gameNode.h"
#include "characterCtrl.h"

class skyBox;

class testScene : public gameNode
{
public:
	testScene();
	~testScene();

	HRESULT init();
	void	release();
	void	update();
	void	render();

private:

	GROUPLIST mapGroupList_1;
	GROUPLIST mapGroupList_2;
	characterCtrl* controller;
	skyBox* skybox;
};

