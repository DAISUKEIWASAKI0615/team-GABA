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
	LoadDivGraph(_T("画像/モナー.png"), 3, 3, 1, 64, 64,pGraph,false);
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
	//座標変換
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

	// 移動量の初期化
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

	//限界値チェック
	if (chips->cameraX < SCREEN_SIZE_X / 2)chips->cameraX = SCREEN_SIZE_X / 2;
	if (chips->cameraY < SCREEN_SIZE_Y / 2)chips->cameraY = SCREEN_SIZE_Y / 2;
	if (chips->cameraX > (MAP_WIDTH *CHIP_SIZE - SCREEN_SIZE_X / 2)) chips->cameraX = (MAP_WIDTH*CHIP_SIZE - SCREEN_SIZE_X / 2);
	if (chips->cameraY > (MAP_HEIGHT*CHIP_SIZE - SCREEN_SIZE_Y / 2)) chips->cameraY = (MAP_HEIGHT*CHIP_SIZE - SCREEN_SIZE_Y / 2);


	// 移動量に基づいてキャラクタの座標を移動
	CharMove(&pos.x, &pos.y, &move, MoveX, MoveY, CHAR_SIZE, &jumpFlg);
	if (pos.x < -14)
	{
		pos.x = -14;
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
		if (chips->MapHitCheck(*X + 14, *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// 右下のチェック、もしブロックの上辺に着いていたら落下を止める
		if (chips->MapHitCheck(*X - 10 + Size, *Y + Size, &Dummy, &MoveY) == 3) *DownSP = 0.0F;

		// 左上のチェック、もしブロックの下辺に当たっていたら落下させる
		if (chips->MapHitCheck(*X + 14, *Y , &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// 右上のチェック、もしブロックの下辺に当たっていたら落下させる
		if (chips->MapHitCheck(*X - 10 + Size, *Y , &Dummy, &MoveY) == 4) *DownSP *= -1.0F;

		// 上下移動成分を加算
		*Y += MoveY;
	}

	// 後に左右移動成分だけでチェック
	{
		// 左下のチェック
		chips->MapHitCheck(*X + 14, *Y + Size, &MoveX, &Dummy);

		// 右下のチェック
		chips->MapHitCheck(*X - 10 + Size, *Y + Size, &MoveX, &Dummy);

		// 左上のチェック
		chips->MapHitCheck(*X + 14, *Y, &MoveX, &Dummy);

		// 右上のチェック
		chips->MapHitCheck(*X - 10 + Size, *Y , &MoveX, &Dummy);

		// 左右移動成分を加算
		*X += MoveX;
	}

	// 接地判定
	{
		// キャラクタの左下と右下の下に地面があるか調べる
		if ((chips->GetChipParam(*X + 17, *Y + Size + 1.0F) == 3 || chips->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 3) ||
			(chips->GetChipParam(*X + 17, *Y + Size + 1.0F) == 0 || chips->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 0) ||
			(chips->GetChipParam(*X + 17, *Y + Size + 1.0F) == 1 || chips->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 1) ||
			(chips->GetChipParam(*X + 17, *Y + Size + 1.0F) == 2 || chips->GetChipParam(*X + Size - 14, *Y + Size + 1.0F) == 2))
		{
			// 足場が在ったら接地中にする
			*JumpFlag = FALSE;
		}
		else
		{
			// 足場が無かったらジャンプ中にする
			*JumpFlag = TRUE;
		}
	}

	// 終了
	return 0;
}
