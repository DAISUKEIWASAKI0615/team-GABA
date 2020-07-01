#include "enemy.h"
#include "map.h"
#include "player.h"

class map* chip;
class player* Pl;

enemy::enemy()
{
	pos = { 100,0 };
	enemy1 = LoadGraph(_T("画像/アキラメロン.png"), false);
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
	//座標変換
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
		if (chip->MapHitCheck(*X + 14, *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// 右下のチェック、もしブロックの上辺に着いていたら落下を止める
		if (chip->MapHitCheck(*X - 10 + Size, *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// 左上のチェック、もしブロックの下辺に当たっていたら落下させる
		if (chip->MapHitCheck(*X + 14, *Y, &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// 右上のチェック、もしブロックの下辺に当たっていたら落下させる
		if (chip->MapHitCheck(*X - 10 + Size, *Y, &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// 上下移動成分を加算
		*Y += MoveY;
	}

	// 後に左右移動成分だけでチェック
	{
		// 左下のチェック
		chip->MapHitCheck(*X + 14, *Y + Size, &MoveX, &Dummy);

		// 右下のチェック
		chip->MapHitCheck(*X - 10 + Size, *Y + Size, &MoveX, &Dummy);

		// 左上のチェック
		chip->MapHitCheck(*X + 14, *Y, &MoveX, &Dummy);

		// 右上のチェック
		chip->MapHitCheck(*X - 10 + Size, *Y, &MoveX, &Dummy);

		// 左右移動成分を加算
		*X += MoveX;
	}

	 //接地判定
	{
		 //キャラクタの左下と右下の下に地面があるか調べる
		if ((chip->GetChipParam(*X + 17, *Y + Size + 1.0F) == 3 || chip->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 3) ||
			(chip->GetChipParam(*X + 17, *Y + Size + 1.0F) == 0 || chip->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 0) ||
			(chip->GetChipParam(*X + 17, *Y + Size + 1.0F) == 1 || chip->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 1) ||
			(chip->GetChipParam(*X + 17, *Y + Size + 1.0F) == 2 || chip->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 2))
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
