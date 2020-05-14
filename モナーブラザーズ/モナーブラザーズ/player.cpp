#pragma comment (lib, "xinput.lib")

#include "player.h"
#include "DxLib.h"
#include "keycheck.h"

player::player()
{
	pos = { 200,100 };
	LoadDivGraph(_T("画像/モナー.png"), 3, 3, 1, 64, 64,pGraph,false);
	speed = 0;
	dire = RIGHT;
	jumpFlg = false;
	oldY = pos.y;
	f = 2;
}

player::~player()
{
}

void player::Draw()
{
	if(dire == RIGHT)
	{
		DrawGraph(pos.x, pos.y, pGraph[0], true);
	}
	else
	{
		DrawTurnGraph(pos.x, pos.y, pGraph[0], true);
	}
}

void player::Update()
{
	runFlg = false;
	if (newKey[P1_RIGHT] || key & PAD_INPUT_RIGHT)
	{
		dire = RIGHT;
	}
	else if (newKey[P1_LEFT] || key & PAD_INPUT_LEFT)
	{
		dire = LEFT;
	}
	key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (jumpFlg == false)
	{
		if ((trgKey[P1_UP] || (key & PAD_INPUT_A)))
		{
			jumpFlg = true;
			for (int h = 0; h < 30; h++)
			{
				move = (pos.y - oldY) + h;
				pos.y -= move;
			}
		}

	}
	//if (newKey[P1_DOWN])
	//{
	//	pos.y += speed;
	//}
	if (newKey[P1_LEFT]|| key & PAD_INPUT_LEFT)
	{
		runFlg = true;
		/*移動中で逆方向に動いたら*/
		if (dire == RIGHT)
		{
			if (pos.x < SCREEN_SIZE_X / 2)speed += 0.7;
			else pos.x += 0;
			if (speed == 0)
			{
				dire = RIGHT;
			}
		}
		else
		{
			speed += -0.5;
			if (speed < -VELOCITY_X_MAX)
			{
				speed = -VELOCITY_X_MAX;
			}

		}
		/*if (pos.x < SCREEN_SIZE_X / 2-32)*/pos.x += speed;
		//else pos.x += 0;
	}
	else if (newKey[P1_RIGHT]|| key & PAD_INPUT_RIGHT)
	{
		runFlg = true;
		/*移動中で逆方向に動いたら*/
		if (dire == LEFT)
		{
			speed += -0.7;
			if (speed == 0)
			{
				dire = LEFT;
			}
		}
		else
		{
			speed += 0.5;
			if (speed > VELOCITY_X_MAX)
			{
				speed = VELOCITY_X_MAX;
			}
		}
		/*if (pos.x < SCREEN_SIZE_X / 2-32)*/pos.x += speed;
		//else pos.x += 0;

	}
	else if(speed!=0)
	{
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
			/*if (pos.x < SCREEN_SIZE_X / 2 - 32)*/pos.x += speed;
			//else pos.x += 0;
		}
	}
	
	if (pos.y + 64 < 500)
	{
		move = (pos.y - oldY) + f;
		if (move > 63)
		{
			move = 63;
		}
		oldY = pos.y;
		pos.y += move;
		f = 2;
	}
	if (pos.y + 64 >= 500) jumpFlg = false;
}
