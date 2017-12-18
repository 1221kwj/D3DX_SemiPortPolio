#include "stdafx.h"
#include "textureManager.h"

textureManager:: textureManager() {}
textureManager::~textureManager() {}

void textureManager::init()
{
	if ( !textureMap.empty() ) textureMap.clear();
	if ( !imageInfoMap.empty() ) imageInfoMap.clear();
}

void textureManager::release()
{
	for each( auto iter in textureMap ) SAFE_RELEASE( iter.second );
	
	textureMap.clear();
	imageInfoMap.clear();
}

LPDIRECT3DTEXTURE9 textureManager::GetTexture( const char * fullPath, OUT D3DXIMAGE_INFO * imageInfo )
{
	return GetTexture( std::string( fullPath ), imageInfo );
}

LPDIRECT3DTEXTURE9 textureManager::GetTexture( std::string fullPath, OUT D3DXIMAGE_INFO * imageInfo )
{
	if ( imageInfo ) {
		if ( textureMap.find( fullPath ) == textureMap.end() ||
			imageInfoMap.find( fullPath ) == imageInfoMap.end() ) {
			SAFE_RELEASE( textureMap[ fullPath ] );
			D3DXCreateTextureFromFileExA(
				DEVICE,
				fullPath.c_str(),
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT,
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_FILTER_NONE,
				D3DX_DEFAULT,
				0,//D3DCOLOR_XRGB(255, 255, 255),
				&imageInfoMap[ fullPath ],
				NULL,
				&textureMap[ fullPath ] );
		}
		*imageInfo = imageInfoMap[ fullPath ];
	}
	else
	{
		if ( textureMap.find( fullPath ) == textureMap.end() ) {
			D3DXCreateTextureFromFileA(
				DEVICE,
				fullPath.c_str(),
				&textureMap[ fullPath ] );
		}
	}

	return textureMap[ fullPath ];
}


