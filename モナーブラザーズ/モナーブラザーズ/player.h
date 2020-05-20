#pragma once
#include "base.h"

#define CHAR_SIZE 64                  // プレイヤーのサイズ
#define JUMP_POWER 9.0                // キャラのジャンプ力
#define G 0.3                         // キャラに掛かる重力加速度
#define VELOCITY_X_MAX 10	// ﾌﾟﾚｲﾔｰ最大速度

typedef enum
{
	LEFT,
	RIGHT
}DIRE;

class player :public base
{
public:
	player();
	virtual ~player();

	void Draw();
	void Update();

	int pGraph[3];
	bool runFlg;
	bool deathFlg;
	int CharMove(float *X, float *Y, float *DownSP,float MoveX, float MoveY, float Size, bool *JumpFlag);

private:
	bool jumpFlg;
	DIRE dire;
	int key;
	float move;
	float f;
	int anim;
	int animCnt;
	float DownSp;
	VECTOR oldPos;
	VECTOR view;
};