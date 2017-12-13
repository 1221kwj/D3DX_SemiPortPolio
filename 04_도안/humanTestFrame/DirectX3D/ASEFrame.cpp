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

	//srt���ְ�
	localTM = rotaMat * transMat;
	//���� ��Ʈ������ �־���
	worldTM = localTM;
	
	//�θ� ������ 
	if (parent)
	{
		//�θ��� �����Ʈ������ ������
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
	//���̵�ƼƼ�� �ʱ�ȭ
	D3DXMatrixIdentity(&mat);
	//���Ͱ� ����ִٸ�, �ڱ��ڽ��� �� �־��ش�
	if (posTrackList.empty())
	{
		mat._41 = localTM._41;
		mat._42 = localTM._42;
		mat._43 = localTM._43;

		return;
	}
	//�����Ӱ��� ���ٰ� �ٽ� ó������ ���ư��� ó�� ������ �۾����� ó�� ���� ��ǥ�� �־��ش�
	if (nKeyFrame <= posTrackList.front().n)
	{
		mat._41 = posTrackList.front().v.x;
		mat._42 = posTrackList.front().v.y;
		mat._43 = posTrackList.front().v.z;

		return;
	}
	//�����Ӱ��� ��� ���� �߿� ������ ������ ũ�� ������ ���� ��ǥ�� �־��ش�
	if (nKeyFrame >= posTrackList.back().n)
	{
		mat._41 = posTrackList.back().v.x;
		mat._42 = posTrackList.back().v.y;
		mat._43 = posTrackList.back().v.z;

		return;
	}
	//��� ���ǿ� �ش����� ������
	//���� �������� ������� ã�� ��
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
	//���� ��ġ
	float t = (nKeyFrame - posTrackList[nPrev].n) / (float)(posTrackList[nNext].n - posTrackList[nPrev].n);
	//���� ��ġ�� ������������ �ؼ� ���� �־���
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
