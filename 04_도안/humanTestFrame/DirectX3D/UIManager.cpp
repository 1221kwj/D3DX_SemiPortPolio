#include "stdafx.h"
#include "UIManager.h"


UIManager::UIManager()	
{
}


UIManager::~UIManager()
{
}

void UIManager::Init()
{
	D3DXIMAGE_INFO stImageInfo;
	LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTexture( "./UI/panel-info.png.png", &stImageInfo );

	UIImageView* imageView = new UIImageView;
	imageView->SetTexture( pTexture );
	imageView->SetSize( D3DXVECTOR2( stImageInfo.Width, stImageInfo.Height ) );
	imageView->SetLocalPos( D3DXVECTOR3( 100, 100, 0 ) );
	imageView->SetDebugRender( true );
	uiRoot = imageView;

	UITextView* pTextView = new UITextView;
	pTextView->SetText( "Test Quest." );
	pTextView->SetSize( D3DXVECTOR2( 312, 200 ) );
	pTextView->SetLocalPos( D3DXVECTOR3( 100, 100, 0 ) );
	pTextView->SetFont( g_pFontManager->GetFont( FontManager::QUEST ) );
	pTextView->SetDebugRender( true );
	pTextView->SetTextFormat( DT_CENTER | DT_VCENTER | DT_WORDBREAK );
	pTextView->SetColor( D3DCOLOR_XRGB( 255, 255, 0 ) );
	pTextView->SetTag( TEXTVIEW );
	uiRoot->AddChild( pTextView );

	UIButton* button = NULL;

	button = new UIButton;
	button->SetTexture( "./UI/btn-med-up.png.png",
		"./UI/btn-med-over.png.png",
		"./UI/btn-med-down.png.png" );
	button->SetLocalPos( D3DXVECTOR3( 135, 330, 0 ) );
	button->SetDebugRender( true );
	button->SetButtonDelegate( this );
	button->SetTag( BUTTON1 );
	uiRoot->AddChild( button );

	button = new UIButton;
	button->SetTexture( "./UI/btn-med-up.png.png",
		"./UI/btn-med-over.png.png",
		"./UI/btn-med-down.png.png" );
	button->SetLocalPos( D3DXVECTOR3( 135, 400, 0 ) );
	button->SetDebugRender( true );
	button->SetButtonDelegate( this );
	button->SetTag( BUTTON2 );
	uiRoot->AddChild( button );

	D3DXCreateSprite( D3DDevice, &sprite );



}

void UIManager::Update()
{
	if ( uiRoot ) uiRoot->Update();
}

void UIManager::Render()
{
	if ( uiRoot ) uiRoot->Render( sprite );
}

void UIManager::Destroy()
{
	SAFE_RELEASE( sprite );
}

void UIManager::OnClick( UIButton * pSender )
{
	UITextView* textView = ( UITextView* ) uiRoot->GetChildByTag( TEXTVIEW );
	if ( pSender->GetTag() == BUTTON1 )
		textView->SetText( "button1 selected" );
	else
		textView->SetText( "button2 selected" );

}
