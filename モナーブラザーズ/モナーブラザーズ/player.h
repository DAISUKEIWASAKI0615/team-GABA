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

private:
	bool jumpFlg;
	DIRE dire;
	int key;
	float move;
	float f;
	float oldY;
};