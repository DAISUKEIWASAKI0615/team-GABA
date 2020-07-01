#pragma once
#include "base.h"

class enemy :public base
{
public:
	enemy();
	~enemy();
	int enemy1;
	int enemy2;
	VECTOR enemyPos;

	void Draw();
	void Update();
	int CharMove(float *X, float *Y, float *DownSP, float MoveX, float MoveY, float Size, bool *downFlg);

private:
	VECTOR oldPos;
	float move;
	float f;
	VECTOR view;
	bool downFlg;
};

