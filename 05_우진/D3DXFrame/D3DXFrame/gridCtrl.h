#pragma once
#include "singletonBase.h"

class gridCtrl : public singletonBase<gridCtrl>
{
private:
	vector<VertexPC> vertexList;

public:
	gridCtrl();
	~gridCtrl();

	void init( void );
	void release( void );
	void render( void );

	GETSET( bool, isRender, IsRender );
};

