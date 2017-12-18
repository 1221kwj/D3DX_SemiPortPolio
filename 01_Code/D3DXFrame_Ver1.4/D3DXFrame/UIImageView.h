#pragma once
#include "UIObject.h"

class UIImageView : public UIObject
{
public:
	UIImageView();
	virtual ~UIImageView();

	GETSET( D3DCOLOR, color, Color );
	GETSET_REF( LPDIRECT3DTEXTURE9, texture, Texture );

	virtual void render( LPD3DXSPRITE sprite ) override;
};

