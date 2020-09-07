#include "player.h"
#include "DxLib.h"
#include "keycheck.h"
#include "map.h"
#include "enemy.h"

player::player()
{
}

player::~player()
{
}

void player::Init()
{
	pos = { 200,SCREEN_SIZE_Y-320 };
	LoadDivGraph(_T("画像/モナー.png"), 5, 5, 1, 64, 64,pGraph,false);
	speed = 0;
	dire = RIGHT;
	jumpFlg = false;
	f = 2;
	anim = 0;
	animCnt = 5;
	oldPos = pos;
	move = 0;
	DownSp = 0;
	view = pos;
	deathFlg = false;
	dropFlg = false;
	jumpCnt = 0;
	vy = 0;
	gr = 0;
	crearFlg = false;
	deathCnt = 0;
	goalCnt = 0;
	goalFlg = false;
	sound1 = LoadSoundMem(_T("sound/jump06.mp3"));
	sound2= LoadSoundMem(_T("sound/nc67702.wav"));
	sound3 = LoadSoundMem(_T("sound/nc124898.wav"));
}

void player::Draw()
{
	//座標変換
	view.x = lpMap.WorldToScreenX(pos.x);
	view.y = lpMap.WorldToScreenY(pos.y);

	int charAnim[] = {0,1};
	if (goalFlg != true)
	{
		if (dire == RIGHT)
		{
			if (--animCnt <= 0)
			{
				anim++;
				animCnt = 5;
				anim %= 2;
			}
			if (deathFlg == true)DrawGraph(view.x, view.y, pGraph[4], true);
			else if (dropFlg == true)DrawGraph(view.x, view.y, pGraph[3], true);
			else if (jumpFlg == true && dropFlg == false)DrawGraph(view.x, view.y, pGraph[2], true);
			else if (runFlg == true)DrawGraph(view.x, view.y, pGraph[charAnim[anim]], true);
			else DrawGraph(view.x, view.y, pGraph[0], true);
		}
		else if (dire == LEFT)
		{
			if (--animCnt <= 0)
			{
				anim++;
				animCnt = 5;
				anim %= 2;
			}
			if (deathFlg == true)DrawTurnGraph(view.x, view.y, pGraph[4], true);
			else if (dropFlg == true)DrawTurnGraph(view.x, view.y, pGraph[3], true);
			else if (jumpFlg == true && dropFlg == false)DrawTurnGraph(view.x, view.y, pGraph[2], true);
			else if (runFlg == true)DrawTurnGraph(view.x, view.y, pGraph[charAnim[anim]], true);
			else DrawGraph(view.x, view.y, pGraph[0], true);
		}
	}
}

void player::Update()
{
	runFlg = false;
	if (deathFlg != true && crearFlg != true)
	{
		// 移動量の初期化
		MoveX = 0.0F;
		MoveY = 0.0F;
		key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		if ((newKey[P1_B] || (key & PAD_INPUT_A)))
		{
			if (jumpFlg == false)
			{
				jumpFlg = true;
				DownSp = -35;
				PlaySoundMem(sound1, DX_PLAYTYPE_BACK);
			}
			gr = 1.5;
		}
		else gr = 2;
		if (jumpFlg == true && dropFlg == false)
		{
			if ((trgKey[P1_A] || (key & PAD_INPUT_B)))
			{
				MoveY = 0;
				dropFlg = true;
			}
		}

		if (jumpFlg == true && dropFlg == true)
		{
			MoveX = 0;
			gr += 3;
		}

		if (newKey[P1_LEFT] || (key & PAD_INPUT_LEFT))
		{
			dire = LEFT;
			runFlg = true;
			speed += -0.3;
			if (speed < -VELOCITY_X_MAX)
			{
				speed = -VELOCITY_X_MAX;
			}
			if (dropFlg == true)speed = 0;
		}
		else if (newKey[P1_RIGHT] || (key & PAD_INPUT_RIGHT))
		{
			dire = RIGHT;
			runFlg = true;
			speed += 0.3;
			if (speed > VELOCITY_X_MAX)
			{
				speed = VELOCITY_X_MAX;
			}
			if (dropFlg == true)speed = 0;
		}
		else if (speed != 0)
		{
			jumpFlg = true;
			if (dire == LEFT) {
				speed += 0.3;
				if (speed > 0)
				{
					speed = 0;
				}
				if (dropFlg == true)speed = 0;
			}
			else if (dire == RIGHT)
			{
				speed += -0.3;
				if (speed < 0)
				{
					speed = 0;
				}
				if (dropFlg == true)speed = 0;
			}
		}
		DownSp += gr;
		MoveX = speed;
		MoveY = DownSp;

		lpMap.cameraX = pos.x + (CHIP_SIZE / 2);

		//限界値チェック
		if (lpMap.cameraX < SCREEN_SIZE_X / 2)lpMap.cameraX = SCREEN_SIZE_X / 2;
		if (lpMap.cameraY < SCREEN_SIZE_Y / 2)lpMap.cameraY = SCREEN_SIZE_Y / 2;
		if (lpMap.cameraX > (MAP_WIDTH  * CHIP_SIZE - SCREEN_SIZE_X / 2))lpMap.cameraX = (MAP_WIDTH  * CHIP_SIZE - SCREEN_SIZE_X / 2);
		if (lpMap.cameraY > (MAP_HEIGHT * CHIP_SIZE - SCREEN_SIZE_Y / 2))lpMap.cameraY = (MAP_HEIGHT * CHIP_SIZE - SCREEN_SIZE_Y / 2);

		// 移動量に基づいてキャラクタの座標を移動
		CharMove(&pos.x, &pos.y, &DownSp, MoveX, MoveY, CHAR_SIZE, &jumpFlg);

		if (pos.x < -14)
		{
			pos.x = -14;
		}
		if (pos.y > SCREEN_SIZE_Y)
		{
			deathFlg = true;
			playerStock--;
		}
		if (lpMap.GetChipParam(pos.x, pos.y) == 25 ||
			lpMap.GetChipParam(pos.x, pos.y) == 26 ||
			lpMap.GetChipParam(pos.x, pos.y) == 27 ||
			lpMap.GetChipParam(pos.x, pos.y) == 28 ||
			lpMap.GetChipParam(pos.x, pos.y) == 29 ||
			lpMap.GetChipParam(pos.x, pos.y) == 30 ||
			lpMap.GetChipParam(pos.x, pos.y) == 31 ||
			lpMap.GetChipParam(pos.x, pos.y) == 32)
		{
			pos.x = (MAP_WIDTH * CHIP_SIZE) - (12 * CHIP_SIZE) - 32;
			crearFlg = true;
		}
		for (int i = 0; i < ENEMY_COUNT; i++)
		{
			if (((pos.x + 14 > lpEnemy.enemyPos[i].x && pos.x + 14 < lpEnemy.enemyPos[i].x + 64)  ||
				 (pos.x + 54 > lpEnemy.enemyPos[i].x && pos.x + 54 < lpEnemy.enemyPos[i].x + 64)) &&
				 (pos.y + 64 < lpEnemy.enemyPos[i].y && pos.y + 64 > lpEnemy.enemyPos[i].y - 20))
			{
				jumpFlg = false;
				jumpFlg = true;
				if (dropFlg != true)DownSp = -27;
				PlaySoundMem(sound3, DX_PLAYTYPE_BACK);
				gr = 1.5;
				lpEnemy.deathFlg[i] = true;
			}
		}
	}
	else if (deathFlg == true)
	{
		deathCnt++;

		if (pos.y < SCREEN_SIZE_Y)
		{
			if (deathCnt > 10)
			{
				pos.y -= 30;
				gr += 2;
				pos.y += gr;
			}
		}
	}
	else if (crearFlg == true)
	{
		if (pos.x < (MAP_WIDTH*CHIP_SIZE) - (4 * CHIP_SIZE))
		{
			dire = RIGHT;
			MoveX = 0;
			MoveY = 0;
			goalCnt++;
			if (goalCnt > 20)
			{
				goalCnt += 0;
				MoveY = 5;
			}
			if (jumpFlg == false)
			{
				runFlg = true;
				DownSp += 2;
				MoveY = DownSp;
				MoveX = 3;
			}
			CharMove(&pos.x, &pos.y, &DownSp, MoveX, MoveY, CHAR_SIZE, &jumpFlg);
		}
		else
		{
			goalFlg = true;
		}
	}

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		if (((pos.x + 32 > lpEnemy.enemyPos[i].x	  && pos.x + 32 < lpEnemy.enemyPos[i].x + 64)  ||
			 (pos.x + 32 > lpEnemy.enemyPos[i].x	  && pos.x + 32 < lpEnemy.enemyPos[i].x + 64)) &&
			((pos.y + 32 < lpEnemy.enemyPos[i].y + 44 && pos.y + 32 > lpEnemy.enemyPos[i].y + 20)  ||
			 (pos.y + 32 > lpEnemy.enemyPos[i].y + 44 && pos.y + 32 < lpEnemy.enemyPos[i].y + 20)))
		{
			if (deathFlg == false)deathFlg = true;
		}
	}
}

// キャラクタをマップとの当たり判定を考慮しながら移動する
int player::CharMove(float *X, float *Y, float *DownSP,
	float MoveX, float MoveY, float Size, bool *JumpFlag)
{
	float Dummy = 0.0F;

	// キャラクタの左上、右上、左下、右下部分が当たり判定のある
	// マップに衝突しているか調べ、衝突していたら補正する

	// 半分のサイズを算出

	// 先ず上下移動成分だけでチェック
	{
		// 左下のチェック、もしブロックの上辺に着いていたら落下を止める
		if (lpMap.MapHitCheck(*X + 14, *Y + Size, &Dummy, &MoveY,dropFlg) == 3) *DownSP = 0.0F;

		// 右下のチェック、もしブロックの上辺に着いていたら落下を止める
		if (lpMap.MapHitCheck(*X - 10 + Size, *Y + Size, &Dummy, &MoveY, dropFlg) == 3) *DownSP = 0.0F;

		// 左上のチェック、もしブロックの下辺に当たっていたら落下させる
		if (lpMap.MapHitCheck(*X + 14, *Y, &Dummy, &MoveY, jumpFlg) == 4)*DownSP *= -1.0F;

		// 右上のチェック、もしブロックの下辺に当たっていたら落下させる
		if (lpMap.MapHitCheck(*X - 10 + Size, *Y, &Dummy, &MoveY, jumpFlg) == 4)*DownSP *= -1.0F;

		// 上下移動成分を加算
		*Y += MoveY;
	}

	// 後に左右移動成分だけでチェック
	{
		// 左下のチェック
		lpMap.MapHitCheck(*X + 14, *Y + Size, &MoveX, &Dummy,dropFlg);

		// 右下のチェック
		lpMap.MapHitCheck(*X - 10 + Size, *Y + Size, &MoveX, &Dummy, dropFlg);

		// 左上のチェック
		lpMap.MapHitCheck(*X + 14, *Y, &MoveX, &Dummy, jumpFlg);

		// 右上のチェック
		lpMap.MapHitCheck(*X - 10 + Size, *Y, &MoveX, &Dummy, jumpFlg);

		// 左右移動成分を加算
		*X += MoveX;
	}

	// 接地判定
	{
		// キャラクタの左下と右下の下に地面があるか調べる
		if ((lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 3  || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 3)  ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 0  || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 0)  ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 1  || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 1)  ||
		   ((lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 2  || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 2)  &&
		   ((lpMap.GetChipsFlag(*X + 14, *Y + Size + 1.0F) == true) || lpMap.GetChipsFlag(*X + Size - 10, *Y + Size + 1.0F) == true)) ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 10 || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 10) ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 11 || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 11) ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 12 || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 12) ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 13 || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 13) ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 6  || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 6)  ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 4  || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 4)  ||
		   ((lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 5  || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 5)  &&
		   ((lpMap.GetChipsFlag(*X + 14, *Y + Size + 1.0F) == false) || lpMap.GetChipsFlag(*X + Size - 10, *Y + Size + 1.0F) == false)) ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 33 || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 33) ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 33 || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 34) ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 33 || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 35) ||
			(lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 33 || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 36) ||
		   ((lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 37 || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 37) &&
		   ((lpMap.GetChipsFlag(*X + 14, *Y + Size + 1.0F) == true) || lpMap.GetChipsFlag(*X + Size - 10, *Y + Size + 1.0F) == true)))
		{
			// 足場が在ったら接地中にする
			*JumpFlag = false;
			dropFlg = false;
		}
		else
		{
			// 足場が無かったらジャンプ中にする
			*JumpFlag = true;
		}
	}

	// 終了
	return 0;
}

