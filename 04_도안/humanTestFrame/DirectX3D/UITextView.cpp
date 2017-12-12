#include "stdafx.h"
#include "UITextView.h"


UITextView::UITextView()
	: font( nullptr )
	, textFormat( DT_LEFT | DT_TOP | DT_NOCLIP )
	, color( D3DCOLOR_XRGB( 255, 255, 255 ) )
{
}


UITextView::~UITextView()
{
	//SAFE_RELEASE( font );
}


void UITextView::Render( LPD3DXSPRITE sprite )
{
	RECT rc;
	SetRect( &rc,
		worldMatrix._41,
		worldMatrix._42,
		worldMatrix._41 + size.x,
		worldMatrix._42 + size.y );

	font->DrawTextA( nullptr,
		text.c_str(),
		text.length(),
		&rc,
		textFormat,
		color );

	UIObject::Render( sprite );

}
