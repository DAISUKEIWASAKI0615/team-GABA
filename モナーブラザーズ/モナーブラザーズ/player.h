#pragma once
#include "base.h"

#define CHAR_SIZE 64                  // プレイヤーのサイズ
#define JUMP_POWER 9.0                // キャラのジャンプ力
#define G 0.3                         // キャラに掛かる重力加速度
#define VELOCITY_X_MAX 10	// ﾌﾟﾚｲﾔｰ最大速度

#ifndef MY_DEBUG_H_INCLUDED
#define MY_DEBUG_H_INCLUDED

#define DEBUG_BUILD  // enable debug print.

#ifdef DEBUG_BUILD
# define DEBUG_PUTS(str) puts(str)
# define DEBUG_PRINTF(fmt, ...)  printf(fmt, __VA_ARGS__);                   
#else
# define DEBUG_PUTS(str)
# define DEBUG_PRINTF(fmt, ...)
#endif

#endif
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
	VECTOR oldPos;
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
};