#pragma once
class UIObject 
	//: public GameObject
{
public:
	UIObject();
	~UIObject();

	virtual void Update();
	virtual void UpdateWorldTM();
	virtual void UpdateChildren();
	virtual void Render( LPD3DXSPRITE sprite );
	virtual void AddChild( UIObject* );
	virtual void Destroy();
	virtual UIObject* GetChildByTag( int _tag );


protected:
	GETSET( D3DXVECTOR3, localPos, LocalPos );
	GETSET( UIObject*, parent, Parent );
	GETSET( D3DXVECTOR2, size, Size );
	GETSET( bool, isDebugRender, DebugRender );
	GETSET( int, tag, Tag );

	std::vector<UIObject*> childList;
	D3DXMATRIXA16 worldMatrix;

};

