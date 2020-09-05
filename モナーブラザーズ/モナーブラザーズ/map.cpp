#include "map.h"
#include "player.h"
#include "DxLib.h"
#include "enemy.h"

map::map()
{
}

map::~map()
{
}

void map::Init()
{
	LoadDivGraph(_T("�摜/�u���b�N�Q.png"), 15, 5, 3, CHIP_SIZE, CHIP_SIZE, blocks, false);
	LoadDivGraph(_T("�摜/�w�i�`�b�v.png"), 3, 3, 1, CHIP_SIZE, CHIP_SIZE, back, false);
	LoadDivGraph(_T("�摜/�y��.png"), 4, 2, 2, CHIP_SIZE, CHIP_SIZE, pipe, false);
	LoadDivGraph(_T("�摜/�w�i(�R).png"), 6, 3, 2, CHIP_SIZE, CHIP_SIZE, mountain, false);
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
			mapData[y][x] = resetMapData[y][x];
			switch (mapData[y][x])
			{
			case 2:
			case 5:
			case 6:
			case 7:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
				blocksFlg[y][x] = true;
				break;
			default:
				blocksFlg[y][x] = false;
				break;
			}
		}
	}
	cameraX = SCREEN_SIZE_X / 2;
	cameraY = SCREEN_SIZE_Y / 2;
	bgm = LoadSoundMem(_T("sound/bgm_maoudamashii_8bit28.mp3"));
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
			if (stages == STAGE1)
			{
				if (backChips[y][x] == 9)DrawGraph(mapPosX, mapPosY, back[0], false);

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
					gimPosY[1] = mapPosY + moveB[1];
					DrawGraph(mapPosX, gimPosY[1], blocks[4], false);
					break;
				case 7:
					gimPosY[0] = mapPosY + moveB[0];
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
					gimPosY[2] = mapPosY + moveB[2];
					DrawGraph(mapPosX, gimPosY[2], blocks[4], false);
					break;
				case 17:
					gimPosY[3] = mapPosY + moveB[3];
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
					gimPosY[4] = mapPosY + moveB[4];
					DrawGraph(mapPosX, gimPosY[4], blocks[4], false);
					break;
				case 34:
					gimPosY[5] = mapPosY + moveB[5];
					DrawGraph(mapPosX, gimPosY[5], blocks[0], false);
					break;
				case 35:
					gimPosY[6] = mapPosY + moveB[6];
					DrawGraph(mapPosX, gimPosY[6], blocks[0], false);
					break;
				case 36:
					gimPosY[7] = mapPosY + moveB[7];
					DrawGraph(mapPosX, gimPosY[7], blocks[0], false);
					break;;
				case 37:
					gimPosY[8] = mapPosY + moveB[8];
					DrawGraph(mapPosX, gimPosY[8], blocks[4], false);
					break;
				default:
					break;
				}
			}
		}
	}
}

void map::Update()
{
	ChangeVolumeSoundMem(150, bgm);
	if (CheckSoundMem(bgm) == 0)PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
	if (lpPlayer.deathFlg == true || lpPlayer.crearFlg == true)StopSoundMem(bgm);
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

	//���E�l�`�F�b�N
	if (cameraX < SCREEN_SIZE_X / 2)cameraX = SCREEN_SIZE_X / 2;
	if (cameraY < SCREEN_SIZE_Y / 2)cameraY = SCREEN_SIZE_Y / 2;
	if (cameraX > (MAP_WIDTH  * CHIP_SIZE - SCREEN_SIZE_X / 2))cameraX = (MAP_WIDTH  * CHIP_SIZE - SCREEN_SIZE_X / 2);
	if (cameraY > (MAP_HEIGHT * CHIP_SIZE - SCREEN_SIZE_Y / 2))cameraY = (MAP_HEIGHT * CHIP_SIZE - SCREEN_SIZE_Y / 2);

	BlocksEffects(lpPlayer.pos.x, lpPlayer.pos.y, lpPlayer.dropFlg, lpPlayer.deathFlg);

	MapGimmick();
	if (stages == STAGE1)
	{
		if (((lpPlayer.pos.x + 17 > 43 * CHIP_SIZE && lpPlayer.pos.x + 17 < 48 * CHIP_SIZE)  ||
			 (lpPlayer.pos.x + 54 > 43 * CHIP_SIZE && lpPlayer.pos.x + 54 < 48 * CHIP_SIZE)) &&
			 (gimPosY[0] + CHIP_SIZE < lpPlayer.pos.y + CHIP_SIZE && gimPosY[0] + CHIP_SIZE > lpPlayer.pos.y))
		{
			if (lpPlayer.deathFlg == false)lpPlayer.deathFlg = true;
		}
		if (((lpPlayer.pos.x + 17 > 13 * CHIP_SIZE && lpPlayer.pos.x + 17 < 14 * CHIP_SIZE)  ||
			 (lpPlayer.pos.x + 54 > 13 * CHIP_SIZE && lpPlayer.pos.x + 54 < 14 * CHIP_SIZE)) &&
			 (gimPosY[3] + CHIP_SIZE < lpPlayer.pos.y + CHIP_SIZE && gimPosY[3] + CHIP_SIZE > lpPlayer.pos.y))
		{
			if (lpPlayer.deathFlg == false)lpPlayer.deathFlg = true;
		}
		if (((lpPlayer.pos.x + 17 > 68 * CHIP_SIZE && lpPlayer.pos.x + 17 < 69 * CHIP_SIZE)  ||
			 (lpPlayer.pos.x + 54 > 68 * CHIP_SIZE && lpPlayer.pos.x + 54 < 69 * CHIP_SIZE)) &&
			 (gimPosY[5] + CHIP_SIZE < lpPlayer.pos.y + CHIP_SIZE && gimPosY[5] + CHIP_SIZE > lpPlayer.pos.y))
		{
			if (lpPlayer.deathFlg == false)lpPlayer.deathFlg = true;
		}
		if (((lpPlayer.pos.x + 17 > 70 * CHIP_SIZE && lpPlayer.pos.x + 17 < 71 * CHIP_SIZE)  ||
			 (lpPlayer.pos.x + 54 > 70 * CHIP_SIZE && lpPlayer.pos.x + 54 < 71 * CHIP_SIZE)) &&
			 (gimPosY[6] + CHIP_SIZE < lpPlayer.pos.y + CHIP_SIZE && gimPosY[6] + CHIP_SIZE > lpPlayer.pos.y))
		{
			if (lpPlayer.deathFlg == false)lpPlayer.deathFlg = true;
		}
		if (((lpPlayer.pos.x + 17 > 72 * CHIP_SIZE && lpPlayer.pos.x + 17 < 73 * CHIP_SIZE)  ||
			 (lpPlayer.pos.x + 54 > 72 * CHIP_SIZE && lpPlayer.pos.x + 54 < 73 * CHIP_SIZE)) &&
		 	 (gimPosY[7] + CHIP_SIZE < lpPlayer.pos.y + CHIP_SIZE && gimPosY[7] + CHIP_SIZE > lpPlayer.pos.y))
		{
			if (lpPlayer.deathFlg == false)lpPlayer.deathFlg = true;
		}
	}
	//if (lpPlayer.deathFlg == true)StopSoundMem(bgm);
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
	if (stages == STAGE1)
	{
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
			(GetChipParam(afX, afY) == 33 && GetChipsFlag(afX, afY) == true) ||
			(GetChipParam(afX, afY) == 34 && GetChipsFlag(afX, afY) == true) ||
			(GetChipParam(afX, afY) == 35 && GetChipsFlag(afX, afY) == true) ||
			(GetChipParam(afX, afY) == 36 && GetChipsFlag(afX, afY) == true) ||
			(GetChipParam(afX, afY) == 37 && GetChipsFlag(afX, afY) == true))
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
				if (GetChipParam(afX, afY) == 33)blocksFlg[(int)afY / CHIP_SIZE][(int)afX / CHIP_SIZE] = false;

				// �E�ӂɓ��������ƕԂ�
				return 2;
			}

			// �����ɗ�����K���Ȓl��Ԃ�
			return 4;
		}
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
	if (stages == STAGE1)
	{

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
			(GetChipParam(afX, afY) == 33 && GetChipsFlag(afX, afY) == true) ||
			(GetChipParam(afX, afY) == 37 && GetChipsFlag(afX, afY) == true))
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
				// �ړ��ʂ�␳����
				*MoveY = bty - Y - 1.0F;

				// ��ӂɓ��������ƕԂ�
				return 3;
			}

			// ���ӂɓ������Ă����ꍇ
			if (*MoveY < 0.0F)
			{
				// �ړ��ʂ�␳����

				*MoveY = bby - Y + 1.0F;
				// ���ӂɓ��������ƕԂ�
				return 4;
			}

			// ���ӂɓ������Ă����ꍇ
			if (*MoveX > 0.0F)
			{
				// �ړ��ʂ�␳����
				*MoveX = blx - X - 1.0F;
				for (int i = 0; i < ENEMY_COUNT; i++)
				{
					lpEnemy.dire[i] = LEFT;
				}
				// ���ӂɓ��������ƕԂ�
				return 1;
			}

			// �E�ӂɓ������Ă����ꍇ
			if (*MoveX < 0.0F)
			{
				// �ړ��ʂ�␳����
				*MoveX = brx - X + 1.0F;
				for (int i = 0; i < ENEMY_COUNT; i++)
				{
					lpEnemy.dire[i] = RIGHT;
				}

				// �E�ӂɓ��������ƕԂ�
				return 2;
			}

			// �����ɗ�����K���Ȓl��Ԃ�
			return 4;
		}
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
		if (GetChipParam(aX, aY) == 2 && GetChipsFlag(aX, aY) == false)
		{
			int x, y;
			x = (int)aX / CHIP_SIZE;
			y = (int)aY / CHIP_SIZE;
			mapData[y][x] = 9;
		}
		if (GetChipParam(bX, aY) == 2 && GetChipsFlag(bX, aY) == false)
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

void map::MapGimmick()
{
	if (lpPlayer.deathFlg != true)
	{
		if (stages == STAGE1)
		{

			bool Flg[GIMMICK_SIZE];
			for (int i = 0; i < GIMMICK_SIZE; i++)
			{
				Flg[i] = false;
			}
			if (lpPlayer.pos.x > 42 * CHIP_SIZE)
			{
				Flg[0] = true;
				for (int x = 45; x < 49; x++)
				{
					blocksFlg[2][x] = false;
				}
			}
			if (Flg[0] == true)vl[0] += 1.5;

			if (lpPlayer.pos.x + 32 > 11 * CHIP_SIZE && lpPlayer.pos.y > SCREEN_SIZE_Y - 128)
			{
				Flg[1] = true;
				blocksFlg[10][11] = false;
			}
			if (Flg[1] == true)vl[1] += 1.5;

			if (lpPlayer.pos.x + 32 > 51 * CHIP_SIZE && lpPlayer.pos.y > SCREEN_SIZE_Y - 128)Flg[2] = true;
			if (Flg[2] == true)vl[2] += 1.5;

			if ((lpPlayer.pos.x + 32 > 12 * CHIP_SIZE && lpPlayer.pos.x + 32 < 13 * CHIP_SIZE) &&
				(lpPlayer.pos.y < 7 * CHIP_SIZE))
			{
				Flg[3] = true;
				blocksFlg[3][13] = false;
			}
			if (Flg[3] == true)vl[3] += 1.5;

			if ((lpPlayer.pos.x + 32 > 88 * CHIP_SIZE && lpPlayer.pos.x + 32 < 92 * CHIP_SIZE) &&
				lpPlayer.pos.y + CHIP_SIZE > SCREEN_SIZE_Y - 3 * CHIP_SIZE)Flg[4] = true;
			if (Flg[4] == true)vl[4] += 1.5;

			if ((lpPlayer.pos.x + 32 > ((SCREEN_SIZE_X / CHIP_SIZE) * 4 + 3) * CHIP_SIZE && lpPlayer.pos.x + 32 < ((SCREEN_SIZE_X / CHIP_SIZE) * 4 + 4) * CHIP_SIZE) &&
				(lpPlayer.pos.y > SCREEN_SIZE_Y - 4 * CHIP_SIZE))
			{
				Flg[5] = true;
				blocksFlg[7][68] = false;
			}
			if (Flg[5] == true)vl[5] += 1.5;

			if ((lpPlayer.pos.x + 32 > ((SCREEN_SIZE_X / CHIP_SIZE) * 4 + 5) * CHIP_SIZE && lpPlayer.pos.x + 32 < ((SCREEN_SIZE_X / CHIP_SIZE) * 4 + 6) * CHIP_SIZE) &&
				(lpPlayer.pos.y > SCREEN_SIZE_Y - 4 * CHIP_SIZE))
			{
				Flg[6] = true;
				blocksFlg[7][70] = false;
			}
			if (Flg[6] == true)vl[6] += 1.5;

			if ((lpPlayer.pos.x + 32 > ((SCREEN_SIZE_X / CHIP_SIZE) * 4 + 7) * CHIP_SIZE && lpPlayer.pos.x + 32 < ((SCREEN_SIZE_X / CHIP_SIZE) * 4 + 8) * CHIP_SIZE) &&
				(lpPlayer.pos.y > SCREEN_SIZE_Y - 4 * CHIP_SIZE))
			{
				Flg[7] = true;
				blocksFlg[7][72] = false;
			}
			if (Flg[7] == true)vl[7] += 1.5;

			if ((lpPlayer.pos.x + 32 > 96 * CHIP_SIZE && lpPlayer.pos.x + 32 < 98 * CHIP_SIZE) &&
				(lpPlayer.pos.y > SCREEN_SIZE_Y - 4 * CHIP_SIZE && lpPlayer.pos.y < SCREEN_SIZE_Y - 3 * CHIP_SIZE))
			{
				Flg[8] = true;;
				for (int x = 95; x < 103; x++)
				{
					blocksFlg[11][x] = false;
				}
			}
			if (Flg[8] == true)vl[8] += 1.5;

			for (int i = 0; i < GIMMICK_SIZE; i++)
			{
				moveB[i] += vl[i];
			}
		}
	}
}
