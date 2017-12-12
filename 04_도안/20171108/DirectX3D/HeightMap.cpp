#include "stdafx.h"
#include "HeightMap.h"

HeightMap::HeightMap()
	:texture(nullptr)
	,mesh(nullptr)
{
}

HeightMap::~HeightMap()
{
}

void HeightMap::Load(char * fileName, D3DXMATRIXA16 * pmat)
{
	std::vector<VertexPNT> PNTverts;
	PNTverts.resize(VERTEX_DIM * VERTEX_DIM);
	verts.resize(VERTEX_DIM * VERTEX_DIM);
	std::vector<DWORD>indexList;
	std::vector<DWORD> attrList;
	//PNTverts.reserve(VERTEX_DIM * VERTEX_DIM);
	//파싱
	FILE* fp = NULL;
	//read binary
	fopen_s(&fp, fileName, "rb");

	for (int z = 0; z < VERTEX_DIM; z++)
	{
		for (int x = 0; x < VERTEX_DIM; x++)
		{
			int index = z * VERTEX_DIM + x;
			float y = ((unsigned char)fgetc(fp)) / 10.0f;

			PNTverts[index].p = D3DXVECTOR3(x, y, z);
			PNTverts[index].n = D3DXVECTOR3(0, 1, 0);
			PNTverts[index].t = D3DXVECTOR2(x / (float)TILE_N, z / (float)TILE_N);

			verts[index] = PNTverts[index].p;
		}
	}
	fclose(fp);
	//인덱스
	indexList.reserve(TILE_N * TILE_N * (2 * 3));

	for (int z = 0; z < TILE_N; z++)
	{
		for (int x = 0; x < TILE_N; x++)
		{
			int _0 = z * VERTEX_DIM + x;
			int _1 = (z + 1) * VERTEX_DIM + x;
			int _2 = z * VERTEX_DIM + (x + 1);
			int _3 = (z + 1) * VERTEX_DIM + (x + 1);

			indexList.push_back(_0);
			indexList.push_back(_1);
			indexList.push_back(_2);

			indexList.push_back(_3);
			indexList.push_back(_2);
			indexList.push_back(_1);
		}
	}
	//노말
	for (int z = 1; z < TILE_N ; z++)
	{
		for (int x = 1; x < TILE_N; x++)
		{
			int index = z * VERTEX_DIM + x;
			int left = index - 1;
			int right = index + 1;
			int top = index + VERTEX_DIM;
			int bottom = index - VERTEX_DIM;
			//아래에서 위를 바라보는 벡터
			D3DXVECTOR3 BT = verts[top] - verts[bottom];
			//왼쪽에서 오른쪽을 바라보는 벡터
			D3DXVECTOR3 LR = verts[right] - verts[left];
			D3DXVECTOR3 n;
			//외적
			D3DXVec3Cross(&n, &BT, &LR);
			D3DXVec3Normalize(&n, &n);

			PNTverts[index].n = n;
		}
	}

	//Mesh create
	//면은 삼각형 두개로 구성되어 있기 때문에 x2
	D3DXCreateMeshFVF(TILE_N * TILE_N * 2, VERTEX_DIM * VERTEX_DIM, D3DXMESH_MANAGED | D3DXMESH_32BIT, VertexPNT::FVF, D3DDevice, &mesh);
	
	VertexPNT* vertexBuffer = nullptr;
	mesh->LockVertexBuffer(0, (LPVOID*)&vertexBuffer);
	memcpy(vertexBuffer, &PNTverts[0], PNTverts.size() * sizeof(VertexPNT));
	mesh->UnlockVertexBuffer();

	//D3DXMESH_32BIT를 사용할때에는 주의할것
	//일반 것에서 사용 할 시 오버플로우 난다고하심
	//32BIT이기 때문에 DWORD사용
	DWORD* indexBuffer = nullptr;
	mesh->LockIndexBuffer(0, (LPVOID*)&indexBuffer);
	memcpy(indexBuffer, &indexList[0], indexList.size() * sizeof(DWORD));
	mesh->UnlockIndexBuffer();

	//모든 폴리곤이 같은 값을 가지기 때문에 0값 넣어주기
	DWORD* attrBuffer = nullptr;
	mesh->LockAttributeBuffer(0, &attrBuffer);
	ZeroMemory(attrBuffer, (indexList.size() / 3) * sizeof(DWORD));
	mesh->UnlockAttributeBuffer();

	//최적화해주기
	//인접버퍼
	std::vector<DWORD>adjBuffer(mesh->GetNumFaces() * 3);
	//                    Epsilon
	mesh->GenerateAdjacency(0.0f, &adjBuffer[0]);
	mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,					
		&adjBuffer[0], 0, 0, 0);
	
}

bool HeightMap::GetHeight(IN const float & x, OUT float & y, IN const float & z)
{
	if (x > TILE_N || x < 0) return false;
	else if (z > TILE_N || z < 0) return false;
	else
	{
		float dx = x - (int)x;
		float dz = z - (int)z;
		
		D3DXVECTOR3 _0 = verts[(int)z * VERTEX_DIM + (int)x];
		D3DXVECTOR3 _1 = verts[((int)z + 1) * VERTEX_DIM + (int)x];
		D3DXVECTOR3 _2 = verts[(int)z * VERTEX_DIM + (int)x + 1];
		D3DXVECTOR3 _3 = verts[((int)z + 1)* VERTEX_DIM + (int)x + 1];

		//밑 삼각형
		if (dx + dz < 1.0f)
		{
			D3DXVECTOR3 uv = _1 - _0;
			D3DXVECTOR3 lv = _2 - _0;

			D3DXVECTOR3 uvZ = uv * dz;
			D3DXVECTOR3 lvX = lv * dx;

			y = _0.y + (uvZ + lvX).y;

			return true;
		}
		//윗 삼각형
		else
		{	
			D3DXVECTOR3 uv = _2 - _3;
			D3DXVECTOR3 lv = _1 - _3;

			D3DXVECTOR3 uvZ = uv * (1 - dz);
			D3DXVECTOR3 lvX = lv * (1 - dx);

			y = _3.y + (uvZ + lvX).y;

			return true;
		}
		
	}

	return false;
}

void HeightMap::Render()
{
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = material.Diffuse = material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DDevice->SetMaterial(&material);
	D3DDevice->SetTexture(0, texture);
	mesh->DrawSubset(0);
}
