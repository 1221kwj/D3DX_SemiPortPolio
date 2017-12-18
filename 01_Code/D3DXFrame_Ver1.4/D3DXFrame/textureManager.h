#pragma once
#include "singletonBase.h"

class textureManager : public singletonBase <textureManager>
{
private:
	map<string, LPDIRECT3DTEXTURE9>	textureMap;
	map<string, D3DXIMAGE_INFO>		imageInfoMap;

public:
	textureManager();
	~textureManager();

	void init();
	void release();

	LPDIRECT3DTEXTURE9 GetTexture( const char* fullPath, OUT D3DXIMAGE_INFO* imageInfo = nullptr );
	LPDIRECT3DTEXTURE9 GetTexture( string fullPath, OUT D3DXIMAGE_INFO* imageInfo = nullptr );
};

