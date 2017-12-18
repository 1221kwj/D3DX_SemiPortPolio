#pragma once
class Pyramid
{
public:
	Pyramid ();
	~Pyramid ();

	void Init ( D3DXCOLOR c, D3DXMATRIXA16& worldMat);
	
	void Render (D3DXMATRIXA16& worldMat);
	//꼭지점
	D3DXVECTOR3 GetFVertex() { return D3DXVECTOR3(transVertexList[0].p.x, transVertexList[0].p.y, transVertexList[0].p.z); }
	//그외의 점들
	D3DXVECTOR3 GetVertex1() { return D3DXVECTOR3(transVertexList[1].p.x, transVertexList[1].p.y, transVertexList[1].p.z); }
	D3DXVECTOR3 GetVertex2() { return D3DXVECTOR3(transVertexList[2].p.x, transVertexList[2].p.y, transVertexList[2].p.z); }
	D3DXVECTOR3 GetVertex3() { return D3DXVECTOR3(transVertexList[4].p.x, transVertexList[4].p.y, transVertexList[4].p.z); }
	D3DXVECTOR3 GetVertex4() { return D3DXVECTOR3(transVertexList[5].p.x, transVertexList[5].p.y, transVertexList[5].p.z); }
private:
	//std::vector<VertexPC> vertexList;
	vector<VertexPN> vertexList;
	vector<VertexPN> transVertexList;
	D3DXMATRIXA16	worldMatrix;
	D3DMATERIAL9	material;

};

