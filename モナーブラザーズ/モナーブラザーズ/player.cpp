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
}

player::~player()
{
}

void player::Draw()
{

	int charAnim[] = {0,1};
	if(dire == RIGHT)
	{
		if (--animCnt <= 0)
		{
			anim++;
			animCnt = 5;
			anim %= 2;
		}
		if(jumpFlg==true)DrawGraph(pos.x, pos.y, pGraph[2], true);
		else if(runFlg==true)DrawGraph(pos.x, pos.y, pGraph[charAnim[anim]], true);
		else DrawGraph(pos.x, pos.y, pGraph[0], true);

	}
	else
	{
		if (--animCnt <= 0)
		{
			anim++;
			animCnt = 5;
			anim %= 2;
		}
		if (jumpFlg == true)DrawTurnGraph(pos.x, pos.y, pGraph[2], true);
		else if (runFlg == true)DrawTurnGraph(pos.x, pos.y, pGraph[charAnim[anim]], true);
		else DrawGraph(pos.x, pos.y, pGraph[0], true);
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
			//pos.x += speed;
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
			//if (pos.x < SCREEN_SIZE_X / 2-32)pos.x += speed;
			if (pos.x < SCREEN_SIZE_X / 2 - 32)MoveX = speed;
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
				//pos.x += speed;
				MoveX = speed;
			}
			else if (dire == RIGHT)
			{
				speed += -0.5;
				if (speed < 0)
				{
					speed = 0;
				}
				//if (pos.x < SCREEN_SIZE_X / 2 - 32)pos.x += speed;
				if (pos.x < SCREEN_SIZE_X / 2 - 32)MoveX = speed;
			}
		}
	// �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
	//if (chips->GetChipParam(pos.x, pos.y + 64) == 3 && chips->GetChipParam(pos.x + 64, pos.y + 64) == 3)
	//{
	//	jumpFlg = false;
	//}
	//else
	{
		move = (pos.y - oldPos.y) + f;
		if (move > 63)
		{
			move = 63;
		}
		oldPos.y = pos.y;
		//pos.y += move;
		f = 2;

	}
	MoveY = move;

	// �ړ��ʂɊ�Â��ăL�����N�^�̍��W���ړ�
	CharMove(&pos.x, &pos.y, &move, MoveX, MoveY, CHAR_SIZE, &jumpFlg);
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
		if (chips->MapHitCheck(*X , *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
		if (chips->MapHitCheck(*X + Size, *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
		if (chips->MapHitCheck(*X , *Y , &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
		if (chips->MapHitCheck(*X + Size, *Y , &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// �㉺�ړ����������Z
		*Y += MoveY;
	}

	// ��ɍ��E�ړ����������Ń`�F�b�N
	{
		// �����̃`�F�b�N
		chips->MapHitCheck(*X, *Y + Size, &MoveX, &Dummy);

		// �E���̃`�F�b�N
		chips->MapHitCheck(*X + Size, *Y + Size, &MoveX, &Dummy);

		// ����̃`�F�b�N
		chips->MapHitCheck(*X, *Y, &MoveX, &Dummy);

		// �E��̃`�F�b�N
		chips->MapHitCheck(*X + Size, *Y , &MoveX, &Dummy);

		// ���E�ړ����������Z
		*X += MoveX;
	}

	// �ڒn����
	{
		// �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
		if (chips->GetChipParam(*X , *Y + Size + 1.0F) == 3 &&
			chips->GetChipParam(*X + Size, *Y + Size + 1.0F) == 3)
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
