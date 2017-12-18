#pragma once
#include "singletonBase.h"

class fontManager : public singletonBase <fontManager>
{
public:
	enum FONTTYPE
	{
		NORMAL,
		CHAT,
		QUEST
	};

private:
	map<FONTTYPE, LPD3DXFONT> fontMap;

public:
	fontManager();
	~fontManager();

	void init();
	void release();

	LPD3DXFONT GetFont( FONTTYPE fontType );
};

