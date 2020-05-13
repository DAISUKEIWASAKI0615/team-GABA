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

	int pGraph[3];

private:
	bool jumpFlg;
	DIRE dire;
	int key;
	float move;
	float f;
	float oldY;
};