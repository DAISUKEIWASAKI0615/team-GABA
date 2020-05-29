#pragma once
#include "base.h"

#define CHAR_SIZE 64                  // �v���C���[�̃T�C�Y
#define JUMP_POWER 9.0                // �L�����̃W�����v��
#define G 0.3                         // �L�����Ɋ|����d�͉����x
#define VELOCITY_X_MAX 10	// ��ڲ԰�ő呬�x

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

	int pGraph[5];
	bool runFlg;
	bool deathFlg;
	bool dropFlg;
	bool jumpFlg;
	bool crearFlg;
	int CharMove(float *X, float *Y, float *DownSP,float MoveX, float MoveY, float Size, bool *JumpFlag);

	float MoveX, MoveY;
private:
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
	float playerStock;
	int deathCnt;
	int sound1,sound2;
	int goalCnt;
	bool goalFlg;
};