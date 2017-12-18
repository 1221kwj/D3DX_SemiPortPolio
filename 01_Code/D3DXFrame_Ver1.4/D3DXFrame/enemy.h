#pragma once
#include "gameNode.h"

class Pyramid;

enum ENEMY_STATE
{
	STATE_IDLE,
	STATE_WALK,
	STATE_RUN,
	STATE_SPRINT,
	STATE_DIE,
	STATE_ATTACK,
	STATE_JUMPATK,
	STATE_HIT,
	STATE_PRONE
};

struct ENEMY_SET
{
	SkinnedMesh*			mesh;
	ENEMY_STATE				state;
	D3DXVECTOR3				eyeVec;
};

class enemy : public gameNode
{
private:
	ENEMY_SET _enemy;
	vector<VertexPN> vertexList;
	D3DXMATRIXA16	worldMatrix;
	D3DMATERIAL9	material;
	Pyramid*		sightRange;
	vector<D3DXPLANE>_vPlane;
	D3DXVECTOR3 lookAt;
	int num = 0;
public:
	enemy();
	~enemy();

	HRESULT init(char* skinName, D3DXVECTOR3 position);
	void release(void);
	void update(void);
	void render(void);

	void attack();
	void move();
	void die();
	void aniSet();
	void sightSet();
	bool collision();
};

