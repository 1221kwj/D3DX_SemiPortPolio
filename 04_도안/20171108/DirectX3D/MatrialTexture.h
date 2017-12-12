

#pragma once

class MaterialTexture : public Object
{
public:
	MaterialTexture ();
	~MaterialTexture ();

protected:
	//mesh를 위한 레퍼런스 추가
	GETSET(int, mtRef, MTRef);

	GETSET_REF( D3DMATERIAL9, material, Material );
	GETSET_ADD_REF ( LPDIRECT3DTEXTURE9, texture, Texture );

};

