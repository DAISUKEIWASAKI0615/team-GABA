#pragma once
#include "base.h"

#define CHAR_SIZE 64                  // プレイヤーのサイズ
#define JUMP_POWER 9.0                // キャラのジャンプ力
#define G 0.3                         // キャラに掛かる重力加速度
#define VELOCITY_X_MAX 10	// ﾌﾟﾚｲﾔｰ最大速度

#define lpPlayer player::GetInstance()

class player :public base
{
public:
	static player& GetInstance()
	{
		static player s_Instance;
		return s_Instance;
	}
	void Init();
	void Draw();
	void Update();

	int pGraph[5];
	bool runFlg;
	bool deathFlg;
	bool dropFlg;
	bool jumpFlg;
	bool crearFlg;
	bool goalFlg;
	int CharMove(float *X, float *Y, float *DownSP,float MoveX, float MoveY, float Size, bool *JumpFlag);

	float MoveX, MoveY;
	float playerStock;
	int sound1,sound2,sound3;

private:
	player();
	virtual ~player();
	DIRE dire;
	int key;
	float move;
	float f;
	int anim;
	int animCnt;
	float DownSp;
	VECTOR oldPos;
	VECTOR view;
	float jumpCnt;
	float vy;
	float gr;
	int deathCnt;
	int goalCnt;
};