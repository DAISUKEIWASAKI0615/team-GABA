#pragma once
#include "base.h"

#define lpEnemy enemy::GetInstance()
#define ENEMY_COUNT 6

class enemy :public base
{
public:
	static enemy& GetInstance()
	{
		static enemy s_Instance;
		return s_Instance;
	}
	int enemy1;
	int enemy2;
	VECTOR enemyPos[ENEMY_COUNT];
	bool deathFlg[ENEMY_COUNT];
	DIRE dire[ENEMY_COUNT];

	void Init();
	void Draw();
	void Update();
	int CharMove(float *X, float *Y, float *DownSP, float MoveX, float MoveY, float Size, bool *downFlg);

private:
	enemy();
	~enemy();
	VECTOR oldPos[ENEMY_COUNT];
	float move[ENEMY_COUNT];
	float f;
	VECTOR view[ENEMY_COUNT];
	bool downFlg[ENEMY_COUNT];
	float DownSp[ENEMY_COUNT];
	float MoveX[ENEMY_COUNT];
	float MoveY[ENEMY_COUNT];
	float gr[ENEMY_COUNT];
};

