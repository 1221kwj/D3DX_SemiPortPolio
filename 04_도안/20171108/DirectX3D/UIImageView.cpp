#include "stdafx.h"
#include "UIImageView.h"


UIImageView::UIImageView()
	: texture( nullptr )
	, color( D3DCOLOR_XRGB( 255, 255, 255 ) )
{
}


UIImageView::~UIImageView()
{
	//SAFE_RELEASE( texture );
}

void UIImageView::Render( LPD3DXSPRITE sprite )
{
	if ( texture ) {
		RECT rc;
		SetRect( &rc, 0, 0, size.x, size.y );
		sprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
		sprite->SetTransform( &worldMatrix );
		sprite->Draw( texture,
			&rc,
			&D3DXVECTOR3( 0, 0, 0 ),
			&D3DXVECTOR3( 0, 0, 0 ),
			color
		);
		sprite->End();
	}

	UIObject::Render( sprite );

}



