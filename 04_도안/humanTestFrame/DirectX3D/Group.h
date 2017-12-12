#pragma once

#include "MatrialTexture.h"

class Group : public Object
{
public:
	Group ();
	~Group ();

	void Render ();
	void SetVB (); 


protected:
	GETSET_REF ( std::vector<VertexPNT>, PNTVerts, PNTVerts );
	GETSET_ADD_REF ( MaterialTexture*, materialTexture, MaterialTexture );
	

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;
};

