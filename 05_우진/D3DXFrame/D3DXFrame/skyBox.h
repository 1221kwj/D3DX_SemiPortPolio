#pragma once
#include "gameNode.h"

class skyBox : gameNode
{
private:
	vector<VertexPT> skyBoxList;

public:
	skyBox();
	~skyBox();

	HRESULT init();
	void render();
};

