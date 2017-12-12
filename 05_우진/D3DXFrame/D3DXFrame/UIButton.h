#pragma once

#include "UIObject.h"

class UIButton;

class iUIButtonDelegate
{
public:
	virtual void OnClick( UIButton* pSender ) = 0;
};

class UIButton : public UIObject
{
public:
	enum BUTTONSTATE
	{
		BTN_NORMAL,
		BTN_MOUSEOVER,
		BTN_SELECT,
		BTN_NONE
	};


	UIButton();
	virtual ~UIButton();

	virtual void SetTexture( string normal, string mouseover, string selected );
	virtual void update() override;
	virtual void render( LPD3DXSPRITE sprite );
	virtual void SetBTNSize( D3DXVECTOR2 );		//원하는 크기를 특정값으로 조정하고 싶을때
	virtual void SetBTNSizePow( D3DXVECTOR2 );	//원하는 크기로 배율로 조정하고 싶을때

public:
	BUTTONSTATE buttonState;
	LPDIRECT3DTEXTURE9 textures[ BUTTONSTATE::BTN_NONE ];

	GETSET( iUIButtonDelegate*, buttonDelegate, ButtonDelegate );
	
};

