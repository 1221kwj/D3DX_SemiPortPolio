#include "stdafx.h"
#include "enemy.h"
#include "Pyramid.h"


enemy::enemy()
{
	
}


enemy::~enemy()
{
}

HRESULT enemy::init(char* skinName , D3DXVECTOR3 position)
{
	_vPlane.resize(6);
	//적 셋팅
	_enemy.mesh = SKINNEDMESHMANAGER->GetSkinnedMesh(skinName);
	//_enemy.mesh->SetAnimationIndex(1);
	_enemy.mesh->SetPosition(position);
	_enemy.state = STATE_PRONE;
	
	//LPD3DXFRAME root = D3DXFrameFind((D3DXFRAME*)_enemy.mesh->getRoot(), "root");

	LPD3DXFRAME camera = D3DXFrameFind(_enemy.mesh->GetRoot(), "camera_fpp");
	lookAt = D3DXVECTOR3(_enemy.mesh->GetBoundingSphere()->vCenter.x, _enemy.mesh->GetBoundingSphere()->vCenter.y, _enemy.mesh->GetBoundingSphere()->vCenter.z - 5);
	if (camera)
	{
		D3DXMATRIXA16 sMat, rMat, tMat, wMat;
		D3DXMatrixIdentity(&sMat);
		D3DXMatrixIdentity(&rMat);
		D3DXMatrixIdentity(&tMat);

		D3DXMatrixScaling(&sMat, 12.0f, 40.0f, 12.0f);
		D3DXMatrixRotationX(&rMat, D3DX_PI * 2 / 5);
		
		D3DXMatrixTranslation(&tMat,((Bone*)camera)->CombinedTransformationMatrix._41
			, ((Bone*)camera)->CombinedTransformationMatrix._42
			, ((Bone*)camera)->CombinedTransformationMatrix._43);
		
		wMat = sMat * rMat * tMat;

		sightRange = new Pyramid;
		sightRange->Init(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f), wMat);
	}
	

	return S_OK;
}

void enemy::release(void)
{
}

void enemy::update(void)
{
	aniSet();
	sightSet();
	//플레이어가 시야에 들어오면
	//바라보기 및 상태 전환
	if (collision() == true)
	{
		_enemy.state = STATE_IDLE;
		//바라보기
		D3DXVECTOR3 playerPos = SKINNEDMESHMANAGER->GetSkinnedMesh("사람")->GetBoundingSphere()->vCenter;
		D3DXVECTOR3 enemyPos = _enemy.mesh->GetBoundingSphere()->vCenter;
		
		D3DXVECTOR3 iseeu = playerPos - enemyPos;
		D3DXVECTOR3 look = lookAt - enemyPos;

		D3DXVECTOR3 n_look, n_iseeu;

		D3DXVec3Normalize(&n_iseeu, &iseeu);
		D3DXVec3Normalize(&n_look, &look);

		float angle;
		
		if (enemyPos.x >= playerPos.x)
		{
			angle = acosf(D3DXVec3Dot(&look, &iseeu) / (D3DXVec3Length(&look) * D3DXVec3Length(&iseeu)));
		}
		else
		{
			angle = -acosf(D3DXVec3Dot(&look, &iseeu) / (D3DXVec3Length(&look) * D3DXVec3Length(&iseeu)));
		}
		_enemy.mesh->SetAngle(angle);
		//이동
		move();
	}
	else
	{
		_enemy.state = STATE_PRONE;
	}
	

}

void enemy::render(void)
{
	D3DXMATRIXA16 RMat = _enemy.mesh->GetRotateMatrix();
	LPD3DXFRAME camera = D3DXFrameFind(_enemy.mesh->GetRoot(), "camera_fpp");
	if (camera)
	{
		D3DXMATRIXA16 sMat, rMat, tMat, wMat;
		D3DXMatrixIdentity(&sMat);
		D3DXMatrixIdentity(&rMat);
		D3DXMatrixIdentity(&tMat);

		D3DXMatrixScaling(&sMat, 12.0f, 40.0f, 12.0f);
		D3DXMatrixRotationX(&rMat, D3DX_PI /2);

		D3DXMatrixTranslation(&tMat, ((Bone*)camera)->CombinedTransformationMatrix._41
			, ((Bone*)camera)->CombinedTransformationMatrix._42
			, ((Bone*)camera)->CombinedTransformationMatrix._43);

		wMat = sMat * rMat * RMat * tMat;

		
		//sightRange->Render(wMat);
	}
	_enemy.mesh->UpdateAndRender();
	
}

void enemy::attack()
{
}

void enemy::move()
{
	//TIMEMANAGER->getElapsedTime()
}

void enemy::die()
{
	
}

void enemy::aniSet()
{
	switch (_enemy.state)
	{
	case STATE_IDLE:
		_enemy.mesh->SetAnimationIndex(3,true);
		break;
	case STATE_PRONE:
		_enemy.mesh->SetAnimationIndex(2, true);
		break;
	case STATE_DIE:
		_enemy.mesh->SetAnimationIndex(8, true);
		break;
	case STATE_ATTACK:
		break;
	case STATE_HIT:
		_enemy.mesh->SetAnimationIndex(0, true);
		break;
	}
}

void enemy::sightSet()
{
	
}

bool enemy::collision()
{
	D3DXVECTOR3 playerPos = SKINNEDMESHMANAGER->GetSkinnedMesh("사람")->GetBoundingSphere()->vCenter;
	D3DXVECTOR3 enemyPos = _enemy.mesh->GetBoundingSphere()->vCenter;

	D3DXVECTOR3 iseeu = playerPos - enemyPos;

	D3DXVECTOR3 Sight1 = sightRange->GetVertex1();
	D3DXVECTOR3 Sight2 = sightRange->GetVertex2();
	D3DXVECTOR3 Sight3 = sightRange->GetVertex3();
	D3DXVECTOR3 Sight4 = sightRange->GetVertex4();

	D3DXVECTOR3 midSight = (Sight1 + Sight2 + Sight3 + Sight4) / 4;
	D3DXVECTOR3 midSee = midSight - enemyPos;
	
	//5.5f는 대략적 시야 길이
	if (D3DXVec3Length(&iseeu) <= D3DXVec3Length(&midSee))
	{
		if (D3DXVec3Dot(&midSee, &iseeu) / (D3DXVec3Length(&midSee) * D3DXVec3Length(&iseeu)) >= D3DX_PI/6)
		{
			return true;
		}
	}

	return false;
}
