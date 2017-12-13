#include "stdafx.h"
#include "ASELoader.h"
#include <map>

ASELoader::ASELoader(void)
	:root(nullptr)
{
}

ASELoader::~ASELoader(void)
{
}

ASEFrame * ASELoader::Load(IN char * filename, OUT ASEScene * pScene)
{
	if (FAILED(fopen_s(&fileStream, filename, "r"))) assert("파일이 없습니다");
	int nLevel = 0;

	while (1)
	{
		char* szToken = GetToken();

		if (szToken == nullptr) break;
		else if (IsEqual(szToken, ID_SCENE))
		{
			ProcessSCENE(pScene);
		}
		else if (IsEqual(szToken, ID_MATERIAL_LIST))
		{
			ProcessMATERIAL_LIST();
		}
		else if (IsEqual(szToken, ID_GEOMETRY))
		{
			ProcessGEOMOBJECT();
		}
	}

	fclose(fileStream);
	return root;
}

char * ASELoader::GetToken()
{
	//"가 있나 없나 체크
	bool isQuote = false;
	//
	int nReadCount = 0;

	while (true)
	{
		//파일에서 한글자씩 읽어옴
		unsigned char c = fgetc(fileStream);
		if (feof(fileStream)) break;
		//"가 없거나 공백을 만나면
		if (!isQuote && IsWhite(c))
		{
			if (nReadCount == 0) continue;
			else break;
		}
		if (c == '\"')
		{
			//첫번째 "를 만나면 isQuote가 true 두번째 "를 만나면 브레이크
			if (isQuote) break;
			isQuote = true;
			continue;
		}
		m_szToken[nReadCount++] = c;
	}

	if (nReadCount == 0) return NULL;

	m_szToken[nReadCount] = '\0';

	return m_szToken;
}

int ASELoader::GetInteger()
{
	return atoi(GetToken());
}

float ASELoader::GetFloat()
{
	return (float)atof(GetToken());
}

bool ASELoader::IsWhite(char c)
{
	//c가 공백(아스키코드)이면 true, 아니면 false
	return c < 33;
}

bool ASELoader::IsEqual(char * str1, char * str2)
{
	//str1과 str2가 같으면 true, 아니면 false
	return strcmp(str1, str2) == 0;
}

void ASELoader::SkipBlock()
{
	int nLevel = 0;

	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{"))
		{
			++nLevel;
		}

		else if (IsEqual(szToken, "}"))
		{
			--nLevel;
		}

	} while (nLevel > 0);
}

void ASELoader::ProcessMATERIAL_LIST()
{
	int nLevel = 0;

	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_MATERIAL_COUNT))
		{
			int num = GetInteger();
			//materialTextureList.resize(num);
			materialTextureList.reserve(num);
		}

		else if (IsEqual(szToken, ID_MATERIAL))
		{
			int num = GetInteger();
			MaterialTexture* newMaterialTexture = new MaterialTexture;
			ProcessMATERIAL(newMaterialTexture);
			materialTextureList.push_back(newMaterialTexture);
		}

	} while (nLevel > 0);
}

void ASELoader::ProcessMATERIAL(OUT MaterialTexture * pMtlTex)
{
	int nLevel = 0;
	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(material));

	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_AMBIENT)) material.Ambient = D3DXCOLOR(GetFloat(), GetFloat(), GetFloat(), 1.0f);
		else if (IsEqual(szToken, ID_DIFFUSE)) material.Diffuse = D3DXCOLOR(GetFloat(), GetFloat(), GetFloat(), 1.0f);
		else if (IsEqual(szToken, ID_SPECULAR)) material.Specular = D3DXCOLOR(GetFloat(), GetFloat(), GetFloat(), 1.0f);
		else if (IsEqual(szToken, ID_MAP_DIFFUSE))
		{
			pMtlTex->SetMaterial(material);
			ProcessMAP_DIFFUSE(pMtlTex);
		}
	} while (nLevel > 0);
}

void ASELoader::ProcessMAP_DIFFUSE(OUT MaterialTexture * pMtlTex)
{
	int nLevel = 0;

	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_BITMAP))
		{
			char* textureName = GetToken();
			pMtlTex->SetTexture(g_pTextureManager->GetTexture(textureName));
		}
	} while (nLevel > 0);
}

ASEFrame * ASELoader::ProcessGEOMOBJECT()
{
	int nLevel = 0;
	ASEFrame* aseFrame = new ASEFrame;
	
	
	std::string	nodeName;
	std::string	parentName;
	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_NODE_NAME)) nodeName = GetToken();
		else if (IsEqual(szToken, ID_NODE_PARENT)) parentName = GetToken();
		else if (IsEqual(szToken, ID_NODE_TM)) ProcessNODE_TM(aseFrame);
		else if (IsEqual(szToken, ID_MESH)) ProcessMESH(aseFrame);
		else if (IsEqual(szToken, ID_TM_ANIMATION)) ProcessTM_ANIMATION(aseFrame);
		else if (IsEqual(szToken, ID_MATERIAL_REF))
		{
			int mtlRef = GetInteger();
			MaterialTexture* texture = materialTextureList[mtlRef];
			aseFrame->SetMaterialTexture(texture);
		}
	} while (nLevel > 0);

	frameMap.insert(std::pair<std::string, ASEFrame*>(nodeName, aseFrame));
	// 이것들은 파일에서 좌표가 다 이동된 상태의 월드 매트릭스 값을 가지고 있기에
	//원 상태로 돌려놓고서 callLocalTM으로 보내주어야한다 
	//임시 템프 매트릭스
	D3DXMATRIXA16 tempMat;
	//초기화
	D3DXMatrixIdentity(&tempMat);
	//가져온 프레임의 월드 매트릭스의 역행렬을 넣어준당
	D3DXMatrixInverse(&tempMat, NULL, &aseFrame->GetWorldTM());
	//그리고 이 역행렬 값을 각각 트렌스 쿠드와 노말을 해준당
	for (int i = 0; i < aseFrame->GetPNTVerts().size(); i++)
	{
		D3DXVec3TransformCoord(&aseFrame->GetPNTVerts()[i].p, &aseFrame->GetPNTVerts()[i].p, &tempMat);
		D3DXVec3TransformNormal(&aseFrame->GetPNTVerts()[i].n, &aseFrame->GetPNTVerts()[i].n, &tempMat);
	}
	//그리고 넣어준다

	//부모가 존재하지 않음
	if (parentName.empty())
	{
		aseFrame->CalcLocalTM(nullptr);
		root = aseFrame;
	}
	//부모 존재
	else
	{
		aseFrame->CalcLocalTM(frameMap[parentName]);
		frameMap[parentName]->AddChild(aseFrame);
	}

	return aseFrame;
}

void ASELoader::ProcessMESH(OUT ASEFrame * pFrame)
{
	int nLevel = 0;

	std::vector<D3DXVECTOR3> pList;
	std::vector<D3DXVECTOR2> tList;
	std::vector<VertexPNT> vertices;

	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_MESH_NUMVERTEX))
		{
			int size = GetInteger();
			pList.resize(size);
			pList.reserve(size);
		}
		else if (IsEqual(szToken, ID_MESH_NUMFACES))
		{
			int size = GetInteger();
			vertices.resize(size * 3);
			vertices.reserve(size * 3);
		}
		else if (IsEqual(szToken, ID_MESH_NUMTVERTEX))
		{
			int size = GetInteger();
			tList.resize(size);
			tList.reserve(size);
		}
		else if (IsEqual(szToken, ID_MESH_VERTEX_LIST)) ProcessMESH_VERTEX_LIST(pList);
		else if (IsEqual(szToken, ID_MESH_FACE_LIST)) ProcessMESH_FACE_LIST(pList, vertices);
		else if (IsEqual(szToken, ID_MESH_TVERTLIST)) ProcessMESH_TVERTLIST(tList);
		else if (IsEqual(szToken, ID_MESH_TFACELIST)) ProcessMESH_TFACELIST(tList, vertices);
		else if (IsEqual(szToken, ID_MESH_NORMALS)) ProcessMESH_NORMALS(vertices);

	} while (nLevel > 0);

	for(int i = 0; i < vertices.size(); i+=3)
	{
		VertexPNT temp;
		temp = vertices[i + 1];
		vertices[i + 1] = vertices[i + 2];
		vertices[i + 2] = temp;
	}

	pFrame->SetPNTVerts(vertices);
}

void ASELoader::ProcessMESH_VERTEX_LIST(OUT std::vector<D3DXVECTOR3>& vecV)
{
	int nLevel = 0;

	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;

		else if (IsEqual(szToken, ID_MESH_VERTEX))
		{
			int num = GetInteger();
			vecV[num].x = GetFloat();
			vecV[num].z = GetFloat();
			vecV[num].y = GetFloat();
		}

	} while (nLevel > 0);
}

void ASELoader::ProcessMESH_FACE_LIST(IN std::vector<D3DXVECTOR3>& vecV, OUT std::vector<VertexPNT>& vecVertex)
{
	int nLevel = 0;
	std::vector<int> indexList;

	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;

		else if (IsEqual(szToken, "A:"))
		{
			int num = GetInteger();
			indexList.push_back(num);
		}
		else if (IsEqual(szToken, "B:"))
		{
			int num = GetInteger();
			indexList.push_back(num);
		}
		else if (IsEqual(szToken, "C:"))
		{
			int num = GetInteger();
			indexList.push_back(num);
		}
	} while (nLevel > 0);

	for (int i = 0; i < indexList.size(); ++i)
	{
		vecVertex[i].p = vecV[indexList[i]];
	}
}

void ASELoader::ProcessMESH_TVERTLIST(OUT std::vector<D3DXVECTOR2>& vecVT)
{
	int nLevel = 0;

	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_MESH_TVERT))
		{
			int num = GetInteger();

			vecVT[num].x = GetFloat();
			vecVT[num].y = 1 - GetFloat();

			GetFloat();
		}

	} while (nLevel > 0);
}

void ASELoader::ProcessMESH_TFACELIST(IN std::vector<D3DXVECTOR2>& vecVT, OUT std::vector<VertexPNT>& vecVertex)
{
	int nLevel = 0;

	do
	{
		char* szToken = GetToken();

		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_MESH_TFACE))
		{
			int num = GetInteger();
			for (int i = 0; i < 3; i++)
			{
				vecVertex[num * 3 + i].t = vecVT[GetInteger()];
			}
		}
	} while (nLevel > 0);
}

void ASELoader::ProcessMESH_NORMALS(OUT std::vector<VertexPNT>& vecVertex)
{
	int nLevel = 0;
	std::vector<D3DXVECTOR3> nList;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_MESH_VERTEXNORMAL))
		{
			GetInteger();
			float x = GetFloat();
			float z = GetFloat();
			float y = GetFloat();
			nList.push_back(D3DXVECTOR3(x, y, -z));
		}

	} while (nLevel > 0);

	for (int i = 0; i < nList.size(); ++i)
	{
		vecVertex[i].n = nList[i];
	}
}

void ASELoader::ProcessNODE_TM(OUT ASEFrame * pFrame)
{
	int nLevel = 0;
	
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		if (IsEqual(szToken, ID_TM_ROW0)) {
			pFrame->GetWorldTM()._11 = GetFloat(); 
			pFrame->GetWorldTM()._13 = GetFloat(); 
			pFrame->GetWorldTM()._12 = GetFloat();
		}
		else if (IsEqual(szToken, ID_TM_ROW1)) {
			pFrame->GetWorldTM()._31 = GetFloat();
			pFrame->GetWorldTM()._33 = GetFloat();
			pFrame->GetWorldTM()._32 = GetFloat();
		}
		else if (IsEqual(szToken, ID_TM_ROW2)) {
			pFrame->GetWorldTM()._21 = GetFloat();
			pFrame->GetWorldTM()._23 = GetFloat();
			pFrame->GetWorldTM()._22 = GetFloat();
		}
		else if (IsEqual(szToken, ID_TM_ROW3)) {
			pFrame->GetWorldTM()._41 = GetFloat(); 
			pFrame->GetWorldTM()._43 = GetFloat(); 
			pFrame->GetWorldTM()._42 = GetFloat();
		}

	} while (nLevel > 0);
	
}

void ASELoader::ProcessTM_ANIMATION(OUT ASEFrame * pFrame)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_POS_TRACK)) ProcessPOS_TRACK(pFrame);
		else if (IsEqual(szToken, ID_ROT_TRACK)) ProcessROT_TRACK(pFrame);
	} while (nLevel > 0);
}

void ASELoader::ProcessPOS_TRACK(OUT ASEFrame * pFrame)
{
	//선형 보간 쓰기
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_POS_SAMPLE))
		{
			int num = GetInteger();
			
			float x = GetFloat();
			float z = GetFloat();
			float y = GetFloat();

			PositionSample temp;
			temp.n = num;
			temp.v = D3DXVECTOR3(x, y, z);
			pFrame->posTrackList.push_back(temp);
		}
	} while (nLevel > 0);
}

void ASELoader::ProcessROT_TRACK(OUT ASEFrame * pFrame)
{
	RotationSample temp;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_ROT_SAMPLE))
		{
			temp.n = GetInteger();
			temp.q.x = GetFloat();
			temp.q.z = GetFloat();
			temp.q.y = GetFloat();
			temp.q.w = GetFloat();

			temp.q.x *= sin(temp.q.w / 2.0f);
			temp.q.z *= sin(temp.q.w / 2.0f);
			temp.q.y *= sin(temp.q.w / 2.0f);
			temp.q.w = cos(temp.q.w / 2.0f);
			//비어있지 않으면 누적해야한다!!
			if (!pFrame->rotTrackList.empty())
			{
				temp.q = pFrame->rotTrackList.back().q * temp.q;
			}
			pFrame->rotTrackList.push_back(temp);
		}
	} while (nLevel > 0);
}

void ASELoader::ProcessSCENE(OUT ASEScene * pScene)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) ++nLevel;
		else if (IsEqual(szToken, "}")) --nLevel;
		else if (IsEqual(szToken, ID_FIRSTFRAME))
		{
			int num = GetInteger();
			pScene->firstFrame = num;
		}
		else if (IsEqual(szToken, ID_LASTFRAME))
		{
			int num = GetInteger();
			pScene->lastFrame = num;
		}
		else if (IsEqual(szToken, ID_FRAMESPEED))
		{
			int num = GetInteger();
			pScene->frameSpeed = num;
		}
		else if (IsEqual(szToken, ID_TICKSPERFRAME))
		{
			int num = GetInteger();
			pScene->tickPerFrame = num;
		}
	} while (nLevel > 0);
}
