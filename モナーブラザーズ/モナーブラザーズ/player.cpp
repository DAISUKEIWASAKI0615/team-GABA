#pragma comment (lib, "xinput.lib")

#include "player.h"
#include "DxLib.h"
#include "keycheck.h"
#include "map.h"

class map;
map* chips;

player::player()
{
	pos = { 200,100 };
	LoadDivGraph(_T("�摜/���i�[.png"), 3, 3, 1, 64, 64,pGraph,false);
	speed = 0;
	dire = RIGHT;
	jumpFlg = false;
	f = 2;
	anim = 0;
	animCnt = 5;
	oldPos = pos;
	chips = new map;
	DownSp = 0;
	view = pos;
}

player::~player()
{
}

void player::Draw()
{
	//���W�ϊ�
	view.x = chips->WorldToScreenX(pos.x);
	view.y = chips->WorldToScreenY(pos.y);

	int charAnim[] = {0,1};
	if(dire == RIGHT)
	{
		if (--animCnt <= 0)
		{
			anim++;
			animCnt = 5;
			anim %= 2;
		}
		if(jumpFlg==true)DrawGraph(view.x, view.y, pGraph[2], true);
		else if(runFlg==true)DrawGraph(view.x, view.y, pGraph[charAnim[anim]], true);
		else DrawGraph(view.x, view.y, pGraph[0], true);
	}
	else if(dire == LEFT)
	{
		if (--animCnt <= 0)
		{
			anim++;
			animCnt = 5;
			anim %= 2;
		}
		if (jumpFlg == true)DrawTurnGraph(view.x, view.y, pGraph[2], true);
		else if (runFlg == true)DrawTurnGraph(view.x, view.y, pGraph[charAnim[anim]], true);
		else DrawGraph(view.x, view.y, pGraph[0], true);
	}
}

void player::Update()
{
	runFlg = false;
	float MoveX, MoveY;

	// �ړ��ʂ̏�����
	MoveX = 0.0F;
	MoveY = 0.0F;

	key = GetJoypadInputState(DX_INPUT_KEY_PAD1);


	if (jumpFlg == false)
	{
		if ((trgKey[P1_UP] || (key & PAD_INPUT_A)))
		{
			jumpFlg = true;
			for (int h = 0; h < 30; h++)
			{
				move = (pos.y - oldPos.y) + h;
				pos.y -= move;
			}
		}
		MoveY = DownSp;
	}
	if (newKey[P1_LEFT] || key & PAD_INPUT_LEFT)
	{
		dire = LEFT;
		runFlg = true;
		speed += -0.5;
		if (speed < -VELOCITY_X_MAX)
		{
			speed = -VELOCITY_X_MAX;
		}
		MoveX = speed;
	}
	else if (newKey[P1_RIGHT] || key & PAD_INPUT_RIGHT)
	{
		dire = RIGHT;
		runFlg = true;
		speed += 0.5;
		if (speed > VELOCITY_X_MAX)
		{
			speed = VELOCITY_X_MAX;
		}
		MoveX = speed;
	}
	else if (speed != 0)
	{
		jumpFlg = true;
		if (dire == LEFT) {
			speed += 0.5;
			if (speed > 0)
			{
				speed = 0;
			}
			MoveX = speed;
		}
		else if (dire == RIGHT)
		{
			speed += -0.5;
			if (speed < 0)
			{
				speed = 0;
			}
			MoveX = speed;
		}
	}
	move = (pos.y - oldPos.y) + f;
	if (move > 63)
	{
		move = 63;
	}
	oldPos.y = pos.y;
	f = 2;

	MoveY = move;

	chips->cameraX = pos.x + (CHIP_SIZE / 2);

	//���E�l�`�F�b�N
	if (chips->cameraX < SCREEN_SIZE_X / 2)chips->cameraX = SCREEN_SIZE_X / 2;
	if (chips->cameraY < SCREEN_SIZE_Y / 2)chips->cameraY = SCREEN_SIZE_Y / 2;
	if (chips->cameraX > (MAP_WIDTH *CHIP_SIZE - SCREEN_SIZE_X / 2)) chips->cameraX = (MAP_WIDTH*CHIP_SIZE - SCREEN_SIZE_X / 2);
	if (chips->cameraY > (MAP_HEIGHT*CHIP_SIZE - SCREEN_SIZE_Y / 2)) chips->cameraY = (MAP_HEIGHT*CHIP_SIZE - SCREEN_SIZE_Y / 2);


	// �ړ��ʂɊ�Â��ăL�����N�^�̍��W���ړ�
	CharMove(&pos.x, &pos.y, &move, MoveX, MoveY, CHAR_SIZE, &jumpFlg);
	if (pos.x < -14)
	{
		pos.x = -14;
	}
}

// �L�����N�^���}�b�v�Ƃ̓����蔻����l�����Ȃ���ړ�����
int player::CharMove(float *X, float *Y, float *DownSP,
	float MoveX, float MoveY, float Size, bool *JumpFlag)
{
	float Dummy = 0.0F;

	// �L�����N�^�̍���A�E��A�����A�E�������������蔻��̂���
	// �}�b�v�ɏՓ˂��Ă��邩���ׁA�Փ˂��Ă�����␳����

	// �����̃T�C�Y���Z�o

	// �悸�㉺�ړ����������Ń`�F�b�N
	{
		// �����̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
		if (chips->MapHitCheck(*X + 14, *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
		if (chips->MapHitCheck(*X - 10 + Size, *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
		if (chips->MapHitCheck(*X + 14, *Y , &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
		if (chips->MapHitCheck(*X - 10 + Size, *Y , &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// �㉺�ړ����������Z
		*Y += MoveY;
	}

	// ��ɍ��E�ړ����������Ń`�F�b�N
	{
		// �����̃`�F�b�N
		chips->MapHitCheck(*X + 14, *Y + Size, &MoveX, &Dummy);

		// �E���̃`�F�b�N
		chips->MapHitCheck(*X - 10 + Size, *Y + Size, &MoveX, &Dummy);

		// ����̃`�F�b�N
		chips->MapHitCheck(*X + 14, *Y, &MoveX, &Dummy);

		// �E��̃`�F�b�N
		chips->MapHitCheck(*X - 10 + Size, *Y , &MoveX, &Dummy);

		// ���E�ړ����������Z
		*X += MoveX;
	}

	// �ڒn����
	{
		// �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
		if ((chips->GetChipParam(*X + 17, *Y + Size + 1.0F) == 3 || chips->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 3) ||
			(chips->GetChipParam(*X + 17, *Y + Size + 1.0F) == 0 || chips->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 0) ||
			(chips->GetChipParam(*X + 17, *Y + Size + 1.0F) == 1 || chips->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 1) ||
			(chips->GetChipParam(*X + 17, *Y + Size + 1.0F) == 2 || chips->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 2))
		{
			// ���ꂪ�݂�����ڒn���ɂ���
			*JumpFlag = FALSE;
		}
		else
		{
			// ���ꂪ����������W�����v���ɂ���
			*JumpFlag = TRUE;
		}
	}

	// �I��
	return 0;
}
