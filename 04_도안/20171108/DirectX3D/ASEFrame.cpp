#include "stdafx.h"

ASEFrame::ASEFrame()
	:materialTexture(nullptr)
{
	D3DXMatrixIdentity(&worldTM);
}

ASEFrame::~ASEFrame()
{
	SAFE_RELEASE(materialTexture);
}

void ASEFrame::Update(int keyFrame, D3DXMATRIXA16 * parent)
{
	D3DXMATRIXA16 rotaMat;
	D3DXMATRIXA16 transMat;

	CalcLocalTranslationMatrix(keyFrame, transMat);
	CalcLocalRotationMatrix(keyFrame, rotaMat);

	//srt해주고
	localTM = rotaMat * transMat;
	//월드 매트릭스에 넣어줌
	worldTM = localTM;
	
	//부모가 있으면 
	if (parent)
	{
		//부모의 월드매트릭스를 곱해줌
		worldTM *= (*parent);
	}
	
	
	for each (auto it in childList)
	{
		it->Update(keyFrame, &worldTM);
	}
}

void ASEFrame::Render()
{
	//D3DDevice->SetRenderState(D3DRS_FILLMODE, 2);
	D3DDevice->SetTransform(D3DTS_WORLD, &worldTM);
	if (materialTexture)
	{
		
		D3DDevice->SetMaterial(&materialTexture->GetMaterial());
		D3DDevice->SetTexture(0, materialTexture->GetTexture());
		D3DDevice->SetFVF(VertexPNT::FVF);
		D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			PNTVerts.size() / 3,
			&PNTVerts[0],
			sizeof(VertexPNT));
	}
	else
	{
		D3DDevice->SetTexture(0, nullptr);
	}

	for each (auto it in childList)
	{
		it->Render();
	}
}

void ASEFrame::AddChild(ASEFrame * child)
{
	childList.push_back(child);
}

void ASEFrame::Destroy()
{
	for each (auto it in childList)
	{
		it->Destroy();
	}
	//SAFE_RELEASE(materialTexture);
	childList.clear();
}

void ASEFrame::CalcLocalTM(ASEFrame * pParent)
{

	localTM = worldTM;

	if(pParent)
	{
		D3DXMATRIXA16 parentWorld;
		D3DXMatrixIdentity(&parentWorld);
		D3DXMatrixInverse(&parentWorld, NULL, &pParent->GetWorldTM());
		localTM = worldTM * parentWorld;
	}
}

void ASEFrame::CalcLocalTranslationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16 & mat)
{
	//아이덴티티로 초기화
	D3DXMatrixIdentity(&mat);
	//벡터가 비어있다면, 자기자신의 값 넣어준당
	if (posTrackList.empty())
	{
		mat._41 = localTM._41;
		mat._42 = localTM._42;
		mat._43 = localTM._43;

		return;
	}
	//프레임값이 돌다가 다시 처음으로 돌아가서 처음 값보다 작아지면 처음 값의 좌표를 넣어준다
	if (nKeyFrame <= posTrackList.front().n)
	{
		mat._41 = posTrackList.front().v.x;
		mat._42 = posTrackList.front().v.y;
		mat._43 = posTrackList.front().v.z;

		return;
	}
	//프레임값이 계속 진행 중에 마지막 값보다 크면 마지막 값의 좌표를 넣어준다
	if (nKeyFrame >= posTrackList.back().n)
	{
		mat._41 = posTrackList.back().v.x;
		mat._42 = posTrackList.back().v.y;
		mat._43 = posTrackList.back().v.z;

		return;
	}
	//모든 조건에 해당하지 않을때
	//현재 프레임이 어디인지 찾는 식
	int nNext = 0;
	for (size_t i = 0; i < posTrackList.size(); i++)
	{
		if (posTrackList[i].n > nKeyFrame)
		{
			nNext = i;
			break;
		}
	}
	int nPrev = nNext - 1;
	//현재 위치
	float t = (nKeyFrame - posTrackList[nPrev].n) / (float)(posTrackList[nNext].n - posTrackList[nPrev].n);
	//현재 위치를 선형보간으로 해서 값을 넣어줌
	D3DXVECTOR3 v;
	D3DXVec3Lerp(&v, &posTrackList[nPrev].v, &posTrackList[nNext].v, t);
	mat._41 = v.x;
	mat._42 = v.y;
	mat._43 = v.z;
	
	
}

void ASEFrame::CalcLocalRotationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16 & mat)
{
	D3DXMatrixIdentity(&mat);
	if (rotTrackList.empty())
	{
		mat = localTM;
		mat._41 = 0.0f;
		mat._42 = 0.0f;
		mat._43 = 0.0f;

		return;
	}
	if (nKeyFrame <= rotTrackList.front().n)
	{
		D3DXMatrixRotationQuaternion(&mat, &rotTrackList.front().q);
		return;
	}
	if (nKeyFrame >= rotTrackList.back().n)
	{
		D3DXMatrixRotationQuaternion(&mat, &rotTrackList.back().q);
		return;
	}
	
	int nNext = -1;
	for (size_t i = 0; i < rotTrackList.size(); i++)
	{
		if (rotTrackList[i].n > nKeyFrame)
		{
			nNext = i;
			break;
		}
	}
	int nPrev = nNext - 1;
	float t = (nKeyFrame - rotTrackList[nPrev].n) / (float)(rotTrackList[nNext].n - rotTrackList[nPrev].n);
	D3DXQUATERNION q;
	D3DXQuaternionSlerp(&q, &rotTrackList[nPrev].q, &rotTrackList[nNext].q, t);
	D3DXMatrixRotationQuaternion(&mat, &q);
	
}
