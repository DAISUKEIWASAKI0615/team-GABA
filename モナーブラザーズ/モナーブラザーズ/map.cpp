#include "map.h"
#include "DxLib.h"
#include "player.h"

map::map()
{
	LoadDivGraph(_T("�摜/�u���b�N�Q.png"), 15, 5, 3, CHIP_SIZE, CHIP_SIZE, blocks, false);
	LoadDivGraph(_T("�摜/�w�i�`�b�v.png"), 3, 3, 1, CHIP_SIZE, CHIP_SIZE, back, false);
	LoadDivGraph(_T("�摜/�y��.png"), 4, 2, 2, CHIP_SIZE, CHIP_SIZE, pipe, false);
	LoadDivGraph(_T("�摜/�w�i(�R).png"),6, 3, 2, CHIP_SIZE, CHIP_SIZE, mountain, false);
	grass = LoadGraph(_T("�摜/�w�i(��).png"), false);
	castle = LoadGraph(_T("�摜/��.png"), false);
	LoadDivGraph(_T("�摜/��.png"), 8, 1, 8, CHIP_SIZE, CHIP_SIZE, goal, false);
	LoadDivGraph(_T("�摜/����.png"), 2, 2, 1, CHIP_SIZE, CHIP_SIZE, flag, false);
	for (int i = 0; i < GIMMICK_SIZE; i++)
	{
		gimPosY[i] = 0;
		vl[i] = 0;
		moveB[i] = 0;
	}
	centerFlg = false;
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			switch (mapData[y][x])
			{
			case 2:
			case 5:
			case 6:
			case 7:
			case 33:
				blocksFlg[y][x] = true;
				break;
			default:
				blocksFlg[y][x] = false;
				break;
			}
		}
	}
	cameraX = SCREEN_SIZE_X/2;
	cameraY = SCREEN_SIZE_Y / 2;
	bgm = LoadSoundMem(_T("sound/bgm_maoudamashii_8bit28.mp3"));
}

map::~map()
{
}

void map::Draw()
{
	for (int y = MAP_HEIGHT; y >= 0; y--)
	{
		for (int x = MAP_WIDTH; x >= 0; x--)
		{
			mapPosX = x * CHIP_SIZE;
			mapPosY = y * CHIP_SIZE;
			mapPosX = WorldToScreenX(mapPosX);
			mapPosY = WorldToScreenY(mapPosY);
			for (int i = 0; i < GIMMICK_SIZE; i++)
			{
				gimPosY[i] = mapPosY + moveB[i];
			}
			if(backChips[y][x]==9)DrawGraph(mapPosX, mapPosY, back[0], false);

			switch (mapData[y][x])
			{
			case 0:
				DrawGraph(mapPosX, mapPosY, blocks[0], false);
				break;
			case 1:
				DrawGraph(mapPosX, mapPosY, blocks[1], false);
				break;
			case 2:
				DrawGraph(mapPosX, mapPosY, blocks[2], false);
				break;
			case 3:
				DrawGraph(mapPosX, mapPosY, blocks[4], false);
				break;
			case 4:
				DrawGraph(mapPosX, mapPosY, blocks[3], false);
				break;
			case 6:
				DrawGraph(mapPosX, gimPosY[1], blocks[4], false);
				break;
			case 7:
				DrawGraph(mapPosX, gimPosY[0], blocks[2], false);
				break;
			case 8:
				if (centerFlg == false)DrawGraph(mapPosX, mapPosY, flag[0], true);
				else if (centerFlg == true)DrawGraph(mapPosX, mapPosY, flag[1], true);
				break;
			case 10:
				DrawGraph(mapPosX, mapPosY, pipe[0], true);
				break;
			case 11:
				DrawGraph(mapPosX, mapPosY, pipe[1], true);
				break;
			case 12:
				DrawGraph(mapPosX, mapPosY, pipe[2], true);
				break;
			case 13:
				DrawGraph(mapPosX, mapPosY, pipe[3], true);
				break;
			case 15:
				DrawGraph(mapPosX, mapPosY, castle, true);
				break;
			case 16:
				DrawGraph(mapPosX, gimPosY[2], blocks[4], false);
				break;
			case 17:
				DrawGraph(mapPosX, gimPosY[3], blocks[0], false);
				break;
			case 18:
				DrawGraph(mapPosX, mapPosY, mountain[0], true);
				break;
			case 19:
				DrawGraph(mapPosX, mapPosY, mountain[1], true);
				break;
			case 20:
				DrawGraph(mapPosX, mapPosY, mountain[2], true);
				break;
			case 21:
				DrawGraph(mapPosX, mapPosY, mountain[3], true);
				break;
			case 22:
				DrawGraph(mapPosX, mapPosY, mountain[4], true);
				break;
			case 23:
				DrawGraph(mapPosX, mapPosY, mountain[5], true);
				break;
			case 24:
				DrawGraph(mapPosX, mapPosY, grass, true);
				break;
			case 25:
				DrawGraph(mapPosX, mapPosY, goal[0], true);
				break;
			case 26:
				DrawGraph(mapPosX, mapPosY, goal[1], true);
				break;
			case 27:
				DrawGraph(mapPosX, mapPosY, goal[2], true);
				break;
			case 28:
				DrawGraph(mapPosX, mapPosY, goal[3], true);
				break;
			case 29:
				DrawGraph(mapPosX, mapPosY, goal[4], true);
				break;
			case 30:
				DrawGraph(mapPosX, mapPosY, goal[5], true);
				break;
			case 31:
				DrawGraph(mapPosX, mapPosY, goal[6], true);
				break;
			case 32:
				DrawGraph(mapPosX, mapPosY, goal[7], true);
				break;
			case 33:
				DrawGraph(mapPosX, gimPosY[4], blocks[4], false);
				break;
			default:
				break;
			}
		}
	}
}

void map::Update(class player *player)
{
	ChangeVolumeSoundMem(150, bgm);
	if (CheckSoundMem(bgm) == 0)PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
	if (player->deathFlg == true || player->crearFlg == true)StopSoundMem(bgm);
	VECTOR start;
	VECTOR end;

	//�}�b�v�f�[�^�͈̔́i�J��������MAP_HANI�`�b�v���j
	start.x = (int)cameraX / CHIP_SIZE - (SCREEN_SIZE_X / CHIP_SIZE);
	start.y = (int)cameraY / CHIP_SIZE - (SCREEN_SIZE_Y / CHIP_SIZE);
	end.x = start.x + 2 * 12;
	end.y = start.y + 2 * 12;

	//���E
	if (start.x < 0) start.x = 0; 
	if (start.y < 0)  start.y = 0; 
	if (end.x > MAP_WIDTH)  end.x = MAP_WIDTH; 
	if (end.y > MAP_HEIGHT) end.y = MAP_HEIGHT;

	cameraX = player->pos.x + (CHIP_SIZE / 2);

	//���E�l�`�F�b�N
	if (cameraX < SCREEN_SIZE_X / 2)cameraX = SCREEN_SIZE_X / 2;
	if (cameraY < SCREEN_SIZE_Y / 2)cameraY = SCREEN_SIZE_Y / 2;
	if (cameraX > (MAP_WIDTH *CHIP_SIZE - SCREEN_SIZE_X / 2)) cameraX = (MAP_WIDTH*CHIP_SIZE - SCREEN_SIZE_X / 2);
	if (cameraY > (MAP_HEIGHT*CHIP_SIZE - SCREEN_SIZE_Y / 2)) cameraY = (MAP_HEIGHT*CHIP_SIZE - SCREEN_SIZE_Y / 2);

	BlocksEffects(player->pos.x, player->pos.y, player->dropFlg,player->deathFlg);

	//if (( GetChipParam(player->pos.x + 14, player->pos.y + 64 + 1.0F) == 6 &&
	//   ( GetChipsFlag(player->pos.x + 14, player->pos.y + 64 + 1.0F) == true)))
	//{
	//	 blocksFlg[(int)(player->pos.y+65) / CHIP_SIZE][(int)(player->pos.x + 14) / CHIP_SIZE] = false;
	//}
	MapGimmick(player);
	if (((player->pos.x > 44 * CHIP_SIZE || player->pos.x + CHIP_SIZE > 44 * CHIP_SIZE)&&
		 (player->pos.x < 49 * CHIP_SIZE || player->pos.x + CHIP_SIZE < 49 * CHIP_SIZE))&&
		(gimPosY[0]<player->pos.y&&gimPosY[0]+CHIP_SIZE > player->pos.y))
	{
		player->deathFlg = true;
	}
	if ((player->pos.x > 13 * CHIP_SIZE&&player->pos.x + CHIP_SIZE < 15 * CHIP_SIZE) &&
		(gimPosY[3] < player->pos.y&&gimPosY[3]+CHIP_SIZE > player->pos.y))
	{
		player->deathFlg = true;
	}
	//if (player->deathFlg == true)StopSoundMem(bgm);
}

// �}�b�v�Ƃ̓����蔻��( �߂�l 0:������Ȃ�����  1:���ӂɓ�������  2:�E�ӂɓ�������
//                                                3:��ӂɓ�������  4:���ӂɓ�������
// ���ӁFMoveX �� MoveY �A�ǂ������Е����O����Ȃ��Ƃ܂Ƃ��ɓ��삵�܂���(��)
int map::MapHitCheck(float X, float Y, float *MoveX, float *MoveY,bool flg)
{
	float afX, afY;

	// �ړ��ʂ𑫂�
	afX = X + *MoveX;
	afY = Y + *MoveY;

	// �����蔻��̂���u���b�N�ɓ������Ă��邩�`�F�b�N
	if (GetChipParam(afX, afY) == 3  ||
		GetChipParam(afX, afY) == 0  || 
		GetChipParam(afX, afY) == 1  || 
	   (GetChipParam(afX, afY) == 2  && GetChipsFlag(afX, afY) == true) ||
		GetChipParam(afX, afY) == 4  ||
	   (GetChipParam(afX, afY) == 6  && GetChipsFlag(afX, afY) == true) ||
	   (GetChipParam(afX, afY) == 7  && GetChipsFlag(afX, afY) == true) ||
		GetChipParam(afX, afY) == 10 ||
		GetChipParam(afX, afY) == 11 ||
		GetChipParam(afX, afY) == 12 ||
		GetChipParam(afX, afY) == 13 ||
		GetChipParam(afX, afY) == 5  ||
		(GetChipParam(afX, afY) == 33 && GetChipsFlag(afX, afY) == true))
	{
		float blx, bty, brx, bby;

		// �������Ă�����ǂ��痣���������s��

		// �u���b�N�̏㉺���E�̍��W���Z�o
		blx = (float)((int)afX / CHIP_SIZE) * CHIP_SIZE;        // ���ӂ� X ���W
		brx = (float)((int)afX / CHIP_SIZE + 1) * CHIP_SIZE;    // �E�ӂ� X ���W

		bty = (float)((int)afY / CHIP_SIZE) * CHIP_SIZE;        // ��ӂ� Y ���W
		bby = (float)((int)afY / CHIP_SIZE + 1) * CHIP_SIZE;    // ���ӂ� Y ���W

		// ��ӂɓ������Ă����ꍇ
		if (*MoveY > 0.0F)
		{
			if (GetChipParam(afX, afY) == 5 && GetChipsFlag(afX, afY) == true)return 0;
			// �ړ��ʂ�␳����
			*MoveY = bty - Y - 1.0F;
			if (GetChipParam(afX, afY) == 2 && flg == true)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			if (GetChipParam(afX, afY) == 6)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			if (GetChipParam(afX, afY) == 7)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			if (GetChipParam(afX, afY) == 33)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			// ��ӂɓ��������ƕԂ�
			return 3;
		}

		// ���ӂɓ������Ă����ꍇ
		if (*MoveY < 0.0F)
		{
			// �ړ��ʂ�␳����
			*MoveY = bby - Y + 1.0F;
			if (GetChipParam(afX, afY) == 2)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			if (GetChipParam(afX, afY) == 5)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			if (GetChipParam(afX, afY) == 6)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			if (GetChipParam(afX, afY) == 7)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			if (GetChipParam(afX, afY) == 33)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			// ���ӂɓ��������ƕԂ�
			return 4;
		}

		// ���ӂɓ������Ă����ꍇ
		if (*MoveX > 0.0F)
		{
			if (GetChipParam(afX, afY) == 5 && GetChipsFlag(afX, afY) == true)return 0;
			// �ړ��ʂ�␳����
			*MoveX = blx - X - 1.0F;
			if (GetChipParam(afX, afY) == 6)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			if (GetChipParam(afX, afY) == 7)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			if (GetChipParam(afX, afY) == 33)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;

			// ���ӂɓ��������ƕԂ�
			return 1;
		}

		// �E�ӂɓ������Ă����ꍇ
		if (*MoveX < 0.0F)
		{
				if (GetChipParam(afX, afY) == 5 && GetChipsFlag(afX, afY) == true)return 0;
				// �ړ��ʂ�␳����
				*MoveX = brx - X + 1.0F;
				if (GetChipParam(afX, afY) == 6)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
				if (GetChipParam(afX, afY) == 7)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
				if (GetChipParam(afX, afY) == 33)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;

				// �E�ӂɓ��������ƕԂ�
				return 2;
		}

		// �����ɗ�����K���Ȓl��Ԃ�
		return 4;
	}

	// �ǂ��ɂ�������Ȃ������ƕԂ�
	return 0;
}

int map::MapHitCheck(float X, float Y, float * MoveX, float * MoveY)
{
	float afX, afY;

	// �ړ��ʂ𑫂�
	afX = X + *MoveX;
	afY = Y + *MoveY;

	// �����蔻��̂���u���b�N�ɓ������Ă��邩�`�F�b�N
	if (GetChipParam(afX, afY) == 3 ||
		GetChipParam(afX, afY) == 0 ||
		GetChipParam(afX, afY) == 1 ||
		(GetChipParam(afX, afY) == 2 && GetChipsFlag(afX, afY) == true) ||
		GetChipParam(afX, afY) == 4 ||
		(GetChipParam(afX, afY) == 6 && GetChipsFlag(afX, afY) == true) ||
		(GetChipParam(afX, afY) == 7 && GetChipsFlag(afX, afY) == true) ||
		GetChipParam(afX, afY) == 10 ||
		GetChipParam(afX, afY) == 11 ||
		GetChipParam(afX, afY) == 12 ||
		GetChipParam(afX, afY) == 13 ||
		GetChipParam(afX, afY) == 5 ||
		(GetChipParam(afX, afY) == 33 && GetChipsFlag(afX, afY) == true))
	{
		float blx, bty, brx, bby;

		// �������Ă�����ǂ��痣���������s��

		// �u���b�N�̏㉺���E�̍��W���Z�o
		blx = (float)((int)afX / CHIP_SIZE) * CHIP_SIZE;        // ���ӂ� X ���W
		brx = (float)((int)afX / CHIP_SIZE + 1) * CHIP_SIZE;    // �E�ӂ� X ���W

		bty = (float)((int)afY / CHIP_SIZE) * CHIP_SIZE;        // ��ӂ� Y ���W
		bby = (float)((int)afY / CHIP_SIZE + 1) * CHIP_SIZE;    // ���ӂ� Y ���W

		// ��ӂɓ������Ă����ꍇ
		if (*MoveY > 0.0F)
		{
			//if (GetChipParam(afX, afY) == 5 && GetChipsFlag(afX, afY) == true)return 0;
			// �ړ��ʂ�␳����
			*MoveY = bty - Y - 1.0F;
			//if (GetChipParam(afX, afY) == 2 && flg == true)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			//if (GetChipParam(afX, afY) == 6)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			//if (GetChipParam(afX, afY) == 7)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			//if (GetChipParam(afX, afY) == 33)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			// ��ӂɓ��������ƕԂ�
			return 3;
		}

		// ���ӂɓ������Ă����ꍇ
		if (*MoveY < 0.0F)
		{
			// �ړ��ʂ�␳����
			*MoveY = bby - Y + 1.0F;
			//if (GetChipParam(afX, afY) == 2)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			//if (GetChipParam(afX, afY) == 5)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			//if (GetChipParam(afX, afY) == 6)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			//if (GetChipParam(afX, afY) == 7)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			//if (GetChipParam(afX, afY) == 33)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			// ���ӂɓ��������ƕԂ�
			return 4;
		}

		// ���ӂɓ������Ă����ꍇ
		if (*MoveX > 0.0F)
		{
			//if (GetChipParam(afX, afY) == 5 && GetChipsFlag(afX, afY) == true)return 0;
			// �ړ��ʂ�␳����
			*MoveX = blx - X - 1.0F;
			//if (GetChipParam(afX, afY) == 6)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			//if (GetChipParam(afX, afY) == 7)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			//if (GetChipParam(afX, afY) == 33)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;

			// ���ӂɓ��������ƕԂ�
			return 1;
		}

		// �E�ӂɓ������Ă����ꍇ
		if (*MoveX < 0.0F)
		{
			//if (GetChipParam(afX, afY) == 5 && GetChipsFlag(afX, afY) == true)return 0;
			// �ړ��ʂ�␳����
			*MoveX = brx - X + 1.0F;
			//if (GetChipParam(afX, afY) == 6)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			//if (GetChipParam(afX, afY) == 7)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;
			//if (GetChipParam(afX, afY) == 33)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;

			// �E�ӂɓ��������ƕԂ�
			return 2;
		}

		// �����ɗ�����K���Ȓl��Ԃ�
		return 4;
	}

	// �ǂ��ɂ�������Ȃ������ƕԂ�
	return 0;
}

// �}�b�v�`�b�v�̒l���擾����֐�
int map::GetChipParam(float X, float Y)
{
	int x, y;

	// �����l�֕ϊ�
	x = (int)X / CHIP_SIZE;
	y = (int)Y / CHIP_SIZE;

	 //�}�b�v����͂ݏo�Ă����� 0 ��Ԃ�
	if (x >= MAP_WIDTH || y >= MAP_HEIGHT || x < 0 || y < 0) return 9;

	// �w��̍��W�ɊY������}�b�v�̏���Ԃ�
	return mapData[y][x];
}

 //���[���h���W����X�N���[�����W��
float map::WorldToScreenX(float p)
{
	float x;
	x = (p - cameraX + (SCREEN_SIZE_X / 2));
	return x;
}

float map::WorldToScreenY(float p)
{
	float y;
	y = (p - cameraY + (SCREEN_SIZE_Y / 2));
	return y;
}

void map::BlocksEffects(float X, float Y, bool flg1, bool flg2)
{
	float aX, aY, bX, bY;
	aX = X + 14;
	bX = X + 54;
	aY = Y - 10;
	bY = Y + 84;
	if (flg2 != true)
	{
		//�W�����v�ɂ�锻��
		if (GetChipParam(aX, aY) == 0)
		{
			int x, y;
			x = (int)aX / CHIP_SIZE;
			y = (int)aY / CHIP_SIZE;
			mapData[y][x] = 1;
		}
		if (GetChipParam(bX, aY) == 0)
		{
			int x, y;
			x = (int)bX / CHIP_SIZE;
			y = (int)aY / CHIP_SIZE;
			mapData[y][x] = 1;
		}
		if (GetChipParam(aX, aY) == 2 && GetChipsFlag(aX, aY) == true)
		{
			int x, y;
			x = (int)aX / CHIP_SIZE;
			y = (int)aY / CHIP_SIZE;
			mapData[y][x] = 9;
		}
		if (GetChipParam(bX, aY) == 2 && GetChipsFlag(bX, aY) == true)
		{
			int x, y;
			x = (int)bX / CHIP_SIZE;
			y = (int)aY / CHIP_SIZE;
			mapData[y][x] = 9;
		}
		if (GetChipParam(aX, aY) == 5 && GetChipsFlag(aX, aY) == true)
		{
			int x, y;
			x = (int)aX / CHIP_SIZE;
			y = (int)aY / CHIP_SIZE;
			mapData[y][x] = 1;
		}
		if (GetChipParam(bX, aY) == 5 && GetChipsFlag(bX, aY) == true)
		{
			int x, y;
			x = (int)bX / CHIP_SIZE;
			y = (int)aY / CHIP_SIZE;
			mapData[y][x] = 1;
		}
		//�q�b�v�h���b�v�ɂ�锻��
		if (flg1 == true && GetChipParam(aX, bY) == 0)
		{
			int x, y;
			x = (int)aX / CHIP_SIZE;
			y = (int)bY / CHIP_SIZE;
			mapData[y][x] = 1;
		}
		if (flg1 == true && GetChipParam(bX, bY) == 0)
		{
			int x, y;
			x = (int)bX / CHIP_SIZE;
			y = (int)bY / CHIP_SIZE;
			mapData[y][x] = 1;
		}
		if (flg1 == true && GetChipParam(aX, bY) == 2 && GetChipsFlag(bX, aY) == true)
		{
			int x, y;
			x = (int)aX / CHIP_SIZE;
			y = (int)bY / CHIP_SIZE;
			mapData[y][x] = 9;
		}
		if (flg1 == true && GetChipParam(bX, bY) == 2 && GetChipsFlag(bX, bY) == true)
		{
			int x, y;
			x = (int)bX / CHIP_SIZE;
			y = (int)bY / CHIP_SIZE;
			mapData[y][x] = 9;
		}
		if (GetChipParam(bX, Y + 10) == 8 && centerFlg == false)
		{
			centerFlg = true;
		}
	}
}

bool map::GetChipsFlag(float X, float Y)
{
	int x, y;

	// �����l�֕ϊ�
	x = (int)X / CHIP_SIZE;
	y = (int)Y / CHIP_SIZE;
	if (blocksFlg[y][x]==true)
	{
		return true;
	}
	return false;
}

void map::MapGimmick(player * player)
{
	if (player->deathFlg != true)
	{
		bool Flg[5];
		for (int i = 0; i < GIMMICK_SIZE; i++)
		{
			Flg[i] = false;
		}
		if (player->pos.x > 41 * CHIP_SIZE)Flg[0] = true;
		if (Flg[0] == true)vl[0] += 1.5;
		if (player->pos.x > 11 * CHIP_SIZE&&player->pos.y > SCREEN_SIZE_Y - 128)Flg[1] = true;
		if (Flg[1] == true)vl[1] += 1.5;
		if (player->pos.x > 51 * CHIP_SIZE&&player->pos.y > SCREEN_SIZE_Y - 128)Flg[2] = true;
		if (Flg[2] == true)vl[2] += 1.5;
		if ((player->pos.x > 11 * CHIP_SIZE&&player->pos.x < 13 * CHIP_SIZE) &&
			(/*player->pos.y > 5 * CHIP_SIZE&&*/player->pos.y < 7 * CHIP_SIZE))Flg[3] = true;
		if (Flg[3] == true)vl[3] += 1.5;
		if ((player->pos.x > 88 * CHIP_SIZE&&player->pos.x < 92 * CHIP_SIZE)&&
			player->pos.y+CHIP_SIZE>SCREEN_SIZE_Y-3*CHIP_SIZE)Flg[4] = true;
		if (Flg[4] == true)vl[4] += 1.5;
		for (int i = 0; i < GIMMICK_SIZE; i++)
		{
			moveB[i] += vl[i];
		}
	}
}
