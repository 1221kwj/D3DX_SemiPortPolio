#include "stdafx.h"
#include "Group.h"


Group::Group ()
	: materialTexture(nullptr)
{
}


Group::~Group ()
{
	SAFE_RELEASE ( materialTexture );
	SAFE_RELEASE ( vertexBuffer );
}

void Group::Render ()
{
	DWORD dwFillMode;
	D3DDevice->GetRenderState ( D3DRS_FILLMODE, &dwFillMode );
	if ( dwFillMode == D3DFILL_WIREFRAME )
	{
		D3DDevice->SetTexture ( 0, NULL );
	}
	else
	{
		D3DDevice->SetTexture ( 0, materialTexture->GetTexture () );
	}

	D3DDevice->SetMaterial ( &materialTexture->GetMaterial() );
	D3DDevice->SetFVF ( VertexPNT::FVF );

	if ( vertexBuffer == nullptr ) {
		D3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLELIST,
			PNTVerts.size () / 3,
			&PNTVerts[0],
			sizeof ( VertexPNT ) );
	}
	else
	{
		D3DDevice->SetStreamSource ( 0, vertexBuffer, 0, sizeof ( VertexPNT ) );
		D3DDevice->DrawPrimitive ( D3DPT_TRIANGLELIST, 0, PNTVerts.size () / 3 );
	}


}

void Group::SetVB ()
{
	if ( PNTVerts.empty () )
		return;

	
	D3DDevice->CreateVertexBuffer (
		PNTVerts.size() * sizeof(VertexPNT),
		0,
		VertexPNT::FVF,
		D3DPOOL_MANAGED,
		&vertexBuffer,
		0
	);

	VertexPNT* verts;
	vertexBuffer->Lock ( 0, 0, ( void** ) &verts, 0 );
	
	for ( int i = 0; i < PNTVerts.size (); ++i ) {
		verts[i] = PNTVerts[i];
	}

	vertexBuffer->Unlock ();

}





