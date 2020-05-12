#include "Dxlib.h"
#include "main.h"
#include "player.h"
#include "keycheck.h"

#define COLOR_MODE 16

typedef enum {
	GMODE_INIT,
	GMODE_TITLE,
	GMODE_GAME,
	GMODE_GAMEOVER
}GMODE;


GMODE gamemode;
int gameCounter;
int fadeCnt;
bool fadeIn;
bool fadeOut;
bool pause;														//一時停止フラグ

class player;

bool SysInit(void);
void GameInit(void);
void GameTitle(void);
void GameMain(void);
void GameOver(void);
void GameMainDraw(void);
void GameTitleDraw(void);
void GameOverDraw(void);
bool FadeInScreen(int fadeStep);
bool FadeOutScreen(int fadeStep);
void HitCheck(void);

player* Player;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (SysInit() == false)
	{
		return -1;
	}

	gamemode = GMODE_INIT;
	Player = new player();

	//ｹﾞｰﾑﾙｰﾌﾟ
	while (ProcessMessage() == 0
		&& CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();
		KeyCheck();

		switch (gamemode)
		{
		case GMODE_INIT:
			GameInit();
			gamemode = GMODE_TITLE;
			break;
		case GMODE_TITLE:
			GameTitle();
			if (fadeIn)
			{
				if (!FadeInScreen(5)) fadeIn = false;
			}
			else if (fadeOut)
			{
				if (!FadeOutScreen(5))
				{
					gamemode = GMODE_GAME;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else
			{
				if (trgKey[START]) fadeOut = true;
			}
			//if (trgKey[START])
			//{
			//	gamemode = GMODE_GAME;
			//}
			break;
		case GMODE_GAME:
			GameMain();
			if (fadeIn)
			{
				if (!FadeInScreen(5)) fadeIn = false;
			}
			break;
		case GMODE_GAMEOVER:
			GameOver();
			if (fadeOut)
			{
				if (!FadeOutScreen(5))
				{
					gamemode = GMODE_INIT;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else
			{
				if (trgKey[START]) fadeOut = true;
			}
			break;
		default:
			break;
		}
		if (!pause) gameCounter++;
		ScreenFlip();
	}
	DxLib_End;
	return 0;
}

bool SysInit(void)
{
	//-----------ｼｽﾃﾑ処理
	SetWindowText(_T("モナーブラザーズ"));
	//640x480ﾄﾞｯﾄ65536色ﾓｰﾄﾞに設定 //8ﾋﾞｯﾄは全て足すと0～255の256種類
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);
	ChangeWindowMode(true);																//true:window　false:ﾌﾙｽｸﾘｰﾝ

	if (DxLib_Init() == -1)																//DXﾗｲﾌﾞﾗﾘの初期化 //DxLibを使った画像の読み込み等はこのﾌﾟﾛｸﾞﾗﾑのあとに
	{
		return false;																	//処理内容 //returnは関数を抜けるときに使われる
	}																					//ifの式はここまででﾜﾝｾｯﾄ
	pause = false;
	fadeIn = false;
	fadeOut = false;
	KeyCheckSystemInit();
	return true;
}

void GameInit(void)
{
}

void GameTitle(void)
{
	GameTitleDraw();
}

void GameTitleDraw(void)
{
	DrawString(0, 0, _T("GameTitle"), 0xFFFFFF);
	Player->Draw();
}

void GameMain(void)
{
	if (trgKey[PAUSE])
	{
		pause = !pause;
	}
	if (pause)
	{
		SetDrawBright(128, 128, 128);
	}
	else
	{
		HitCheck();
		if (trgKey[START]) gamemode = GMODE_GAMEOVER;
	}
	GameMainDraw();
	if (pause)
	{
		SetDrawBright(255, 255, 255);
		DrawString(360, 292, _T("PAUSE"), 0xFFFFFF);
	}
}

void GameMainDraw(void)
{
	DrawFormatString(0, 0, 0xFFFFFF, _T("GameMain:%d"), gameCounter);
}

void GameOver(void)
{
	GameOverDraw();
}

void GameOverDraw(void)
{
	DrawString(0, 0, _T("GameOver"), 0xFFFFFF);
}

bool FadeInScreen(int fadeStep)
{
	fadeCnt += fadeStep;
	if (fadeCnt <= 255)
	{
		SetDrawBright(fadeCnt, fadeCnt, fadeCnt);
		return true;
	}
	else
	{
		SetDrawBright(255, 255, 255);
		fadeCnt = 0;
		return false;
	}
}

bool FadeOutScreen(int fadeStep)
{
	fadeCnt += fadeStep;
	if (fadeCnt <= 255)
	{
		SetDrawBright(255 - fadeCnt, 255 - fadeCnt, 255 - fadeCnt);
		return true;
	}
	else
	{
		SetDrawBright(0, 0, 0);
		fadeCnt = 0;
		return false;
	}
}

void HitCheck(void)
{

}