#pragma once

class UIObject : public Object
{
public:
	UIObject();
	~UIObject();

	virtual void update();
	virtual void updateWorldTM();
	virtual void updateChildren();
	virtual void render( LPD3DXSPRITE sprite );
	virtual void addChild( UIObject* );
	virtual void release();
	virtual UIObject* GetChildByTag( int _tag );

protected:
	GETSET( D3DXVECTOR3,      localPos,    LocalPos );
	GETSET( D3DXVECTOR2,	      size,        Size );
	GETSET( UIObject*  ,        parent,      Parent );
	GETSET( bool       , isDebugRender, DebugRender );
	GETSET( int        ,           tag,         Tag );

	vector<UIObject*> childList;
	D3DXMATRIXA16 worldMatrix;
};

