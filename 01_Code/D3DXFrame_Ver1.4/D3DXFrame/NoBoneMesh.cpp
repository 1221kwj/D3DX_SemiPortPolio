#include "stdafx.h"
#include "NoBoneMesh.h"


NoBoneMesh::NoBoneMesh()
	:positionVec(D3DXVECTOR3(0,0,0))
{
}


NoBoneMesh::~NoBoneMesh()
{
}

void NoBoneMesh::Load(char * szFolder, char * szFile)
{
	HRESULT hr = 0;

	string folder(szFolder);
	string fullpath = szFolder + std::string("\\") + szFile;

	fullName = fullpath;

	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;

	DWORD numMtrls = 0;

	hr = D3DXLoadMeshFromXA(fullpath.c_str(), D3DXMESH_MANAGED, DEVICE, &adjBuffer, &mtrlBuffer, 0, &numMtrls, &mesh);
	//string szFolderName = "./testX/";
	if (FAILED(hr))
	{
		MessageBoxA(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
	}

	if (mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; ++i)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			//
			Mtrls.push_back(mtrls[i].MatD3D);
			if (mtrls[i].pTextureFilename != 0)
			{
				string szFullPath = folder + string(mtrls[i].pTextureFilename);
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFileA(DEVICE, szFullPath.c_str(), &tex);

				Textures.push_back(tex);
			}
			else
			{
				Textures.push_back(0);
			}
		}
	}
	mtrlBuffer->Release();

	hr = mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(), 0, 0, 0);
	adjBuffer->Release();

	if (FAILED(hr))
	{
		MessageBoxA(0, "½ÇÆÐ¿°", 0, 0);
	}
}

void NoBoneMesh::Render()
{
	D3DXMATRIXA16 posMat;
	D3DXMatrixIdentity(&posMat);
	D3DXMatrixTranslation(&posMat, positionVec.x, positionVec.y, positionVec.z);
	DEVICE->SetTransform(D3DTS_WORLD, &posMat);
	for (int i = 0; i < Mtrls.size(); i++)
	{
		DEVICE->SetMaterial(&Mtrls[i]);
		DEVICE->SetTexture(0, Textures[i]);
		mesh->DrawSubset(i);
	}

	D3DXMATRIXA16 idMat;
	D3DXMatrixIdentity(&idMat);

	DEVICE->SetTransform(D3DTS_WORLD, &idMat);
}

void NoBoneMesh::Destroy()
{
	mesh->Release();
	for (int i = 0; i < Textures.size(); i++)
	{
		Textures[i]->Release();
	}
}
