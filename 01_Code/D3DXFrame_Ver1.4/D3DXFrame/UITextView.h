#pragma once
#include "UIObject.h"

class UITextView : public UIObject
{
public:
	UITextView();
	virtual ~UITextView();
	virtual void render( LPD3DXSPRITE sprite ) override;

	GETSET( std::string, text, Text );
	GETSET( DWORD, textFormat, TextFormat );
	GETSET( DWORD, color, Color );
	GETSET( LPD3DXFONT, font, Font );
};

