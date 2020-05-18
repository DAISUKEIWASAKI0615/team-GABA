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

	}
	if (newKey[P1_LEFT]|| key & PAD_INPUT_LEFT)
	{
		dire = LEFT;
		runFlg = true;
		/*移動中で逆方向に動いたら*/
		//if (dire == RIGHT)
		//{
		//	speed += 0.7;
		//}
		//else
		{
			speed += -0.5;
			if (speed < -VELOCITY_X_MAX)
			{
				speed = -VELOCITY_X_MAX;
			}
		}
		pos.x += speed;
	}
	else if (newKey[P1_RIGHT]|| key & PAD_INPUT_RIGHT)
	{
		dire = RIGHT;
		runFlg = true;
		/*移動中で逆方向に動いたら*/
		//if (dire == LEFT)
		//{
		//	speed += -0.7;
		//}
		//else
		{
			speed += 0.5;
			if (speed > VELOCITY_X_MAX)
			{
				speed = VELOCITY_X_MAX;
			}
		}
		if (pos.x < SCREEN_SIZE_X / 2-32)pos.x += speed;

	}
	else if(speed!=0)
	{
		jumpFlg = true;
		if (dire == LEFT) {
			speed += 0.5;
			if (speed > 0)
			{
				speed = 0;
			}
			pos.x += speed;
		}
		else if (dire == RIGHT)
		{
			speed += -0.5;
			if (speed < 0)
			{
				speed = 0;
			}
			if (pos.x < SCREEN_SIZE_X / 2 - 32)pos.x += speed;

		}
	}

	// キャラクタの左下と右下の下に地面があるか調べる
	if (chips->GetChipParam(pos.x, pos.y + 64) == 3 && chips->GetChipParam(pos.x + 64, pos.y + 64) == 3)
	{
		jumpFlg = false;
	}
	else
	{
		move = (pos.y - oldPos.y) + f;
		if (move > 63)
		{
			move = 63;
		}
		oldPos.y = pos.y;
		pos.y += move;
		f = 2;

	}

}

