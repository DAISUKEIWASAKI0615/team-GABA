#include "Dxlib.h"
#include "main.h"
#include "player.h"
#include "keycheck.h"
#include"map.h"
#include"enemy.h"

#define COLOR_MODE 16

typedef enum {
	GMODE_INIT,
	GMODE_TITLE,
	GMODE_GAME,
	GMODE_GAMEOVER
}GMODE;


GMODE gamemode;
int gameCounter;
bool pause;														//一時停止フラグ
int cnt;
int title;
int titleMap[SCREEN_SIZE_Y/CHIP_SIZE][SCREEN_SIZE_X / CHIP_SIZE]
{
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
};

class player;
class map;
class enemy;

bool SysInit(void);
void GameInit(void);
void GameTitle(void);
void GameMain(void);
void GameOver(void);
void GameMainDraw(void);
void GameTitleDraw(void);
void GameOverDraw(void);
void HitCheck(void);
void PlayerDeath(void);

player* Player;
map* Map;
enemy* Enemy;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (SysInit() == false)
	{
		return -1;
	}

	gamemode = GMODE_INIT;

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
			if (trgKey[START])
			{
				gamemode = GMODE_GAME;
			}
			break;
		case GMODE_GAME:
			GameMain();
			if (Player->deathFlg == true)PlayerDeath();
			break;
		case GMODE_GAMEOVER:
			GameOver();
			gamemode = GMODE_INIT;
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
	cnt = 0;
	KeyCheckSystemInit();
	title = LoadGraph(_T("画像/タイトルロゴ.png"), false);
	return true;
}

void GameInit(void)
{
	Player = new player();
	Map = new map();
	Enemy = new enemy();
}

void GameTitle(void)
{
	GameTitleDraw();
}

void GameTitleDraw(void)
{
	DrawString(0, 0, _T("GameTitle"), 0xFFFFFF);
	for (int y = 0; y < SCREEN_SIZE_Y / CHIP_SIZE; y++)
	{
		for (int x = 0; x < SCREEN_SIZE_X / CHIP_SIZE; x++)
		{
			switch (titleMap[y][x])
			{
			case 3:
				DrawGraph(x*CHIP_SIZE, y*CHIP_SIZE, Map->blocks[4], false);
				break;
			case 9:
				DrawGraph(x*CHIP_SIZE, y*CHIP_SIZE, Map->back[0], false);
				break;
			default:
				break;
			}
		}
	}
	DrawGraph(200, SCREEN_SIZE_Y - 128, Player->pGraph[0], true);
	DrawGraph(SCREEN_SIZE_X / 8, SCREEN_SIZE_Y / 6, title, true);
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
		Player->Update();
		Map->Update(Player);
		
		//if (trgKey[START]) gamemode = GMODE_GAMEOVER;
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
	Map->Draw();
	Player->Draw();
	Enemy->Draw();
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

void HitCheck(void)
{

}

void PlayerDeath(void)
{
	cnt++;
	if (cnt > 100)gamemode = GMODE_GAMEOVER;
}