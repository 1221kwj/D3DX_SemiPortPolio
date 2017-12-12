#include "stdafx.h"
#include "UIButton.h"

UIButton::UIButton()
	: buttonState	( BTN_NORMAL )
	, buttonDelegate( nullptr )
{
}

UIButton::~UIButton() {}

void UIButton::SetTexture( std::string normalTexture, std::string mouseoverTexture, std::string selectedTexture )
{
	D3DXIMAGE_INFO imageInfo;
	textures[ BTN_NORMAL ]		= TEXTUREMANAGER->GetTexture( normalTexture, &imageInfo );
	textures[ BTN_MOUSEOVER ]	= TEXTUREMANAGER->GetTexture( mouseoverTexture, &imageInfo );
	textures[ BTN_SELECT ]		= TEXTUREMANAGER->GetTexture( selectedTexture, &imageInfo );

	size.x = imageInfo.Width;
	size.y = imageInfo.Height;
}

void UIButton::update()
{
	UIObject::updateWorldTM();

	RECT rc;
	SetRect( &rc,
		worldMatrix._41,
		worldMatrix._42,
		worldMatrix._41 + size.x,
		worldMatrix._42 + size.y
	);

	POINT ptCurrMouse;
	GetCursorPos( &ptCurrMouse );
	ScreenToClient( _hWnd, &ptCurrMouse );
	
	if ( PtInRect( &rc, ptCurrMouse ) )
	{
		if ( KEYMANAGER->isOnceKeyDown( VK_LBUTTON ) )
		{
			if ( buttonState == BTN_MOUSEOVER ) buttonState = BTN_SELECT;
		}
		else
		{
			if ( buttonState == BTN_SELECT )
			{
				if ( buttonDelegate ) buttonDelegate->OnClick( this );
			}
			buttonState = BTN_MOUSEOVER;
		}
	}
	else
	{
		if ( GetKeyState( VK_LBUTTON ) & 0x8000 ) {}
		else buttonState = BTN_NORMAL;
	}

	UIObject::updateChildren();
}

void UIButton::render( LPD3DXSPRITE sprite )
{
	RECT rc;
	SetRect( &rc, 0, 0, size.x, size.y );
	sprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
	
	sprite->SetTransform( &worldMatrix );
	sprite->Draw( 
		textures[ buttonState ],
		&rc,
		&D3DXVECTOR3( 0, 0, 0 ),
		&D3DXVECTOR3( 0, 0, 0 ),
		D3DCOLOR_XRGB( 255, 255, 255 ) );

	sprite->End();

	UIObject::render( sprite );

}

void UIButton::SetBTNSize( D3DXVECTOR2 pSize )
{

}

void UIButton::SetBTNSizePow( D3DXVECTOR2 pSize )
{

}
