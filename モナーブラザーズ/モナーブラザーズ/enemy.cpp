#include "enemy.h"
#include "map.h"
#include "player.h"

class map* chip;
class player* Pl;

enemy::enemy()
{
	pos = { 100,0 };
	enemy1 = LoadGraph(_T("�摜/�A�L��������.png"), false);
	chip = new map;
	Pl = new player;
	speed = 5;
	f = 2;
	downFlg = false;
	enemyPos = { 100,0 };
}

enemy::~enemy()
{
}

void enemy::Draw()
{
	//���W�ϊ�
	view.x = chip->WorldToScreenX(enemyPos.x);
	view.y = chip->WorldToScreenY(enemyPos.y);

	DrawGraph(view.x, view.y, enemy1, true);
	//DrawGraph(pos.x, pos.y, enemy1, true);

}

void enemy::Update()
{
	float MoveX = 0;
	float MoveY = 0;
	MoveX = -speed;
	//move = (pos.y - oldPos.y) + f;
	//if (move > 63)
	//{
	//	move = 63;
	//}
	//oldPos.y = pos.y;
	//f = 2;

	MoveY = move;


	if (chip->cameraX < SCREEN_SIZE_X / 2)chip->cameraX = SCREEN_SIZE_X / 2;
	if (chip->cameraY < SCREEN_SIZE_Y / 2)chip->cameraY = SCREEN_SIZE_Y / 2;
	if (chip->cameraX > (MAP_WIDTH *CHIP_SIZE - SCREEN_SIZE_X / 2)) chip->cameraX = (MAP_WIDTH*CHIP_SIZE - SCREEN_SIZE_X / 2);
	if (chip->cameraY > (MAP_HEIGHT*CHIP_SIZE - SCREEN_SIZE_Y / 2)) chip->cameraY = (MAP_HEIGHT*CHIP_SIZE - SCREEN_SIZE_Y / 2);


	//CharMove(&enemyPos.x, &enemyPos.y, 0, MoveX, MoveY, CHIP_SIZE, &downFlg);
}

// �L�����N�^���}�b�v�Ƃ̓����蔻����l�����Ȃ���ړ�����
int enemy::CharMove(float *X, float *Y, float *DownSP,float MoveX, float MoveY, float Size, bool *downFlg)
{
	float Dummy = 0.0F;

	// �L�����N�^�̍���A�E��A�����A�E�������������蔻��̂���
	// �}�b�v�ɏՓ˂��Ă��邩���ׁA�Փ˂��Ă�����␳����

	// �����̃T�C�Y���Z�o

	// �悸�㉺�ړ����������Ń`�F�b�N
	{
		// �����̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
		if (chip->MapHitCheck(*X + 14, *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
		if (chip->MapHitCheck(*X - 10 + Size, *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
		if (chip->MapHitCheck(*X + 14, *Y, &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
		if (chip->MapHitCheck(*X - 10 + Size, *Y, &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// �㉺�ړ����������Z
		*Y += MoveY;
	}

	// ��ɍ��E�ړ����������Ń`�F�b�N
	{
		// �����̃`�F�b�N
		chip->MapHitCheck(*X + 14, *Y + Size, &MoveX, &Dummy);

		// �E���̃`�F�b�N
		chip->MapHitCheck(*X - 10 + Size, *Y + Size, &MoveX, &Dummy);

		// ����̃`�F�b�N
		chip->MapHitCheck(*X + 14, *Y, &MoveX, &Dummy);

		// �E��̃`�F�b�N
		chip->MapHitCheck(*X - 10 + Size, *Y, &MoveX, &Dummy);

		// ���E�ړ����������Z
		*X += MoveX;
	}

	 //�ڒn����
	{
		 //�L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
		if ((chip->GetChipParam(*X + 17, *Y + Size + 1.0F) == 3 || chip->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 3) ||
			(chip->GetChipParam(*X + 17, *Y + Size + 1.0F) == 0 || chip->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 0) ||
			(chip->GetChipParam(*X + 17, *Y + Size + 1.0F) == 1 || chip->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 1) ||
			(chip->GetChipParam(*X + 17, *Y + Size + 1.0F) == 2 || chip->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 2))
		{
			 //���ꂪ�݂�����ڒn���ɂ���
			*downFlg = FALSE;
		}
		else
		{
			 //���ꂪ����������W�����v���ɂ���
			*downFlg = TRUE;
		}
	}

	// �I��
	return 0;
}
