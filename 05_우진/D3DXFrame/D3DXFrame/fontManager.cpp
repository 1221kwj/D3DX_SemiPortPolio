#include "stdafx.h"
#include "fontManager.h"

fontManager:: fontManager() {}
fontManager::~fontManager() {}

void fontManager::init()
{
	if ( !fontMap.empty() ) fontMap.clear();
}

void fontManager::release()
{
	for each( auto iter in fontMap ) SAFE_RELEASE( iter.second );
	fontMap.clear();
}

LPD3DXFONT fontManager::GetFont( FONTTYPE fontType )
{
	if ( fontMap.find( fontType ) == fontMap.end() ) {
		switch ( fontType ) {
		case FONTTYPE::CHAT:
		{

		}
		break;
		case FONTTYPE::NORMAL:
		{
			D3DXFONT_DESC fd;
			ZeroMemory( &fd, sizeof( LPD3DXFONT_DESC ) );

			fd.Height = 20;
			fd.Width = 8;
			fd.Weight = FW_NORMAL;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			//strcpy_s(fd.FaceName, "±¼¸²Ã¼");	
			//AddFontResource("umberto.ttf");
			wcscpy_s( fd.FaceName, L"±¼¸²Ã¼" );
			//strcpy(fd.FaceName, "umberto");

			//D3DXCreateFontIndirect( DEVICE, &fd, &fontMap[ fontType ] );
			D3DXCreateFontIndirect( DEVICE, &fd, &fontMap[ fontType ] );
		}
		break;
		case FONTTYPE::QUEST:
		{
			D3DXFONT_DESC fd;
			ZeroMemory( &fd, sizeof( LPD3DXFONT_DESC ) );

			fd.Height = 25;
			fd.Width = 12;
			fd.Weight = FW_NORMAL;
			fd.Italic = false;
			fd.CharSet = DEFAULT_CHARSET;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.PitchAndFamily = FF_DONTCARE;
			//strcpy_s(fd.FaceName, "±¼¸²Ã¼");	
			//AddFontResource("umberto.ttf");
			wcscpy_s( fd.FaceName, L"±Ã¼­Ã¼" );
			//strcpy(fd.FaceName, "umberto");

			//D3DXCreateFontIndirect( DEVICE, &fd, &fontMap[ fontType ] );
			D3DXCreateFontIndirect( DEVICE, &fd, &fontMap[ fontType ] );
		}
		break;
		}
	}
	return fontMap[ fontType ];
}
