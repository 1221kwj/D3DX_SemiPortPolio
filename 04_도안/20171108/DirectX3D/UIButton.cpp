
#include "stdafx.h"

#include "UIButton.h"


UIButton::UIButton()
	: buttonState( NORMAL )
	, buttonDelegate( nullptr )
{
}


UIButton::~UIButton()
{
}

void UIButton::SetTexture( std::string normalTexture, std::string mouseoverTexture, std::string selectedTexture )
{
	D3DXIMAGE_INFO imageInfo;
	textures[NORMAL] = g_pTextureManager->GetTexture( normalTexture, &imageInfo );
	textures[MOUSEOVER] = g_pTextureManager->GetTexture( mouseoverTexture, &imageInfo );
	textures[SELECT] = g_pTextureManager->GetTexture( selectedTexture, &imageInfo );

	size.x = imageInfo.Width;
	size.y = imageInfo.Height;

}

void UIButton::Update()
{
	UIObject::UpdateWorldTM();

	RECT rc;
	SetRect( &rc,
		worldMatrix._41,
		worldMatrix._42,
		worldMatrix._41 + size.x,
		worldMatrix._42 + size.y
	);

	POINT ptCurrMouse;
	GetCursorPos( &ptCurrMouse );
	ScreenToClient( g_hWnd, &ptCurrMouse );


	if ( PtInRect( &rc, ptCurrMouse ) )
	{
		if ( GetKeyState( VK_LBUTTON ) & 0x8000 )
		{
			if ( buttonState == MOUSEOVER )
			{
				buttonState = SELECT;
			}
		}
		else
		{
			if ( buttonState == SELECT )
			{
				// ´­·¶´Ù ¶À.
				if ( buttonDelegate )
					buttonDelegate->OnClick( this );
			}
			buttonState = MOUSEOVER;
		}
	}
	else
	{
		if ( GetKeyState( VK_LBUTTON ) & 0x8000 )
		{

		}
		else
		{
			buttonState = NORMAL;
		}
	}

	UIObject::UpdateChildren();


}

void UIButton::Render( LPD3DXSPRITE sprite )
{
	RECT rc;
	SetRect( &rc, 0, 0, size.x, size.y );
	sprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
	sprite->SetTransform( &worldMatrix );
	sprite->Draw( textures[buttonState],
		&rc,
		&D3DXVECTOR3( 0, 0, 0 ),
		&D3DXVECTOR3( 0, 0, 0 ),
		D3DCOLOR_XRGB( 255, 255, 255 ) );
	sprite->End();

	UIObject::Render( sprite );

}
