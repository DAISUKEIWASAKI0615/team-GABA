#include "enemy.h"
#include "map.h"
#include "player.h"

enemy::enemy()
{
}

enemy::~enemy()
{
}

void enemy::Init()
{
	pos = { 100,0 };
	enemy1 = LoadGraph(_T("画像/アキラメロン.png"), false);
	speed = 1;
	f = 2;
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		deathFlg[i] = false;
		MoveX[i] = 0;
		MoveY[i] = 0;
		downFlg[i] = false;
		DownSp[i] = 0;
		gr[i] = 2;
		dire[i] = LEFT;
	}
	enemyPos[0] = { 900,SCREEN_SIZE_Y - (CHIP_SIZE * 2) };
	enemyPos[1] = { 1600,SCREEN_SIZE_Y - (CHIP_SIZE * 2) };
	enemyPos[2] = { ((SCREEN_SIZE_X / CHIP_SIZE) * 3) * CHIP_SIZE,CHIP_SIZE };
	enemyPos[3] = { ((SCREEN_SIZE_X / CHIP_SIZE) * 4 + 7) * CHIP_SIZE,SCREEN_SIZE_Y - (CHIP_SIZE * 2) };
	enemyPos[4] = { ((SCREEN_SIZE_X / CHIP_SIZE) * 6 + 5) * CHIP_SIZE,SCREEN_SIZE_Y - (CHIP_SIZE * 2) };
	enemyPos[5] = { ((SCREEN_SIZE_X / CHIP_SIZE) * 7 + 8) * CHIP_SIZE, - (CHIP_SIZE * 2) };
}

void enemy::Draw()
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		//座標変換
		view[i].x = lpMap.WorldToScreenX(enemyPos[i].x);
		view[i].y = lpMap.WorldToScreenY(enemyPos[i].y);
		if (deathFlg[i] == false)
		{
			if (dire[i] == LEFT)DrawGraph(view[i].x, view[i].y, enemy1, true);
			if (dire[i] == RIGHT)DrawTurnGraph(view[i].x, view[i].y, enemy1, true);
		}
	}
}

void enemy::Update()
{
	float playerCam = lpPlayer.pos.x + (CHIP_SIZE / 2);
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		if (enemyPos[i].x<(playerCam)+SCREEN_SIZE_X/2+CHIP_SIZE*3&& enemyPos[i].x > (playerCam) - SCREEN_SIZE_X / 2)
		{
			if (deathFlg[i] == false)
			{
				if (dire[i] == LEFT)MoveX[i] = -speed;
				if (dire[i] == RIGHT)MoveX[i] = speed;
				DownSp[i] += gr[i];
				MoveY[i] = DownSp[i];

				CharMove(&enemyPos[i].x, &enemyPos[i].y, &DownSp[i], MoveX[i], MoveY[i], CHIP_SIZE, &downFlg[i]);
			}
			else if (deathFlg[i] == true)
			{
				enemyPos[i] = { 0,0 };
			}
		}
	}
}

// キャラクタをマップとの当たり判定を考慮しながら移動する
int enemy::CharMove(float *X, float *Y, float *DownSP,float MoveX, float MoveY, float Size, bool *downFlg)
{
	float Dummy = 0.0F;

	// キャラクタの左上、右上、左下、右下部分が当たり判定のある
	// マップに衝突しているか調べ、衝突していたら補正する

	// 半分のサイズを算出

	// 先ず上下移動成分だけでチェック
	{
		// 左下のチェック、もしブロックの上辺に着いていたら落下を止める
		if (lpMap.MapHitCheck(*X + 14, *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// 右下のチェック、もしブロックの上辺に着いていたら落下を止める
		if (lpMap.MapHitCheck(*X - 10 + Size, *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// 左上のチェック、もしブロックの下辺に当たっていたら落下させる
		if (lpMap.MapHitCheck(*X + 14, *Y, &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// 右上のチェック、もしブロックの下辺に当たっていたら落下させる
		if (lpMap.MapHitCheck(*X - 10 + Size, *Y, &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// 上下移動成分を加算
		*Y += MoveY;
	}

	// 後に左右移動成分だけでチェック
	{
		// 左下のチェック
		lpMap.MapHitCheck(*X + 14, *Y + Size, &MoveX, &Dummy);

		// 右下のチェック
		lpMap.MapHitCheck(*X - 10 + Size, *Y + Size, &MoveX, &Dummy);

		// 左上のチェック
		lpMap.MapHitCheck(*X + 14, *Y, &MoveX, &Dummy);

		// 右上のチェック
		lpMap.MapHitCheck(*X - 10 + Size, *Y, &MoveX, &Dummy);

		// 左右移動成分を加算
		*X += MoveX;
	}

	 //接地判定
	{
		 //キャラクタの左下と右下の下に地面があるか調べる
		if ((lpMap.GetChipParam(*X + 17, *Y + Size + 1.0F) == 3 || lpMap.GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 3) ||
			(lpMap.GetChipParam(*X + 17, *Y + Size + 1.0F) == 0 || lpMap.GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 0) ||
			(lpMap.GetChipParam(*X + 17, *Y + Size + 1.0F) == 1 || lpMap.GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 1) ||
			(lpMap.GetChipParam(*X + 17, *Y + Size + 1.0F) == 2 || lpMap.GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 2) ||
			(lpMap.GetChipParam(*X + 17, *Y + Size + 1.0F) == 6 || lpMap.GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 6) ||
		   ((lpMap.GetChipParam(*X + 17, *Y + Size + 1.0F) == 7 || lpMap.GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 7) &&
		   ((lpMap.GetChipsFlag(*X + 17, *Y + Size + 1.0F) == true) || lpMap.GetChipsFlag(*X + Size - 14, *Y + Size + 1.0F) == true)) ||
		   ((lpMap.GetChipParam(*X + 14, *Y + Size + 1.0F) == 37 || lpMap.GetChipParam(*X + Size - 10, *Y + Size + 1.0F) == 37) &&
		   ((lpMap.GetChipsFlag(*X + 14, *Y + Size + 1.0F) == true) || lpMap.GetChipsFlag(*X + Size - 10, *Y + Size + 1.0F) == true)))
		{
			 //足場が在ったら接地中にする
			*downFlg = FALSE;
		}
		else
		{
			 //足場が無かったらジャンプ中にする
			*downFlg = TRUE;
		}
	}

	// 終了
	return 0;
}
