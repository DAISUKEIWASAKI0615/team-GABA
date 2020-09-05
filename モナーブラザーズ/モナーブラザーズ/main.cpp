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
	GMODE_STOCK,
}GMODE;

typedef enum {
	STAGE1_CLEAR,
	STAGE2_CLEAR
}CREAR;

GMODE gamemode;
CREAR stageCrear;
int gameCounter;
bool pause;														//一時停止フラグ
int cnt;
int title;
int titleBack[SCREEN_SIZE_Y / CHIP_SIZE][SCREEN_SIZE_X / CHIP_SIZE]
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
	9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9
};

int titleMap[SCREEN_SIZE_Y/CHIP_SIZE][SCREEN_SIZE_X / CHIP_SIZE]
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,4,5,6,0,0,
	0,0,0,0,0,1,0,0,0,0,0,7,8,9,0,0,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
};


bool centerSpawn;
int playerstock;
int stockCnt;
int goal;

bool SysInit(void);
void GameInit(void);
void GameTitle(void);
void GameStock(void);
void GameStockDraw(void);
void GameMain(void);
void GameMainDraw(void);
void GameTitleDraw(void);
void PlayerDeath(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (SysInit() == false)
	{
		return -1;
	}

	gamemode = GMODE_TITLE;

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
			gamemode = GMODE_STOCK;
			break;
		case GMODE_TITLE:
			GameTitle();
			if (trgKey[START])gamemode = GMODE_INIT;
			break;
		case GMODE_STOCK:
			GameStock();
			stockCnt++;
			if (stockCnt>60)gamemode = GMODE_GAME;
			break;
		case GMODE_GAME:
			GameMain();
			if (lpPlayer.deathFlg == true)PlayerDeath();
			if (lpPlayer.goalFlg == true)
			{
				goal++;
				if (goal > 120)
				{
					centerSpawn = false;
					goal = 0;
					stockCnt = 0;
					goal = 0;
					lpMap.stages = STAGE2;
					gamemode = GMODE_INIT;
				}
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
	KeyCheckSystemInit();
	centerSpawn = false;
	stockCnt = 0;
	goal = 0;
	lpEnemy.Init();
	lpPlayer.Init();
	lpMap.Init();
	lpPlayer.playerStock = 2;
	playerstock = lpPlayer.playerStock;
	title = LoadGraph(_T("画像/タイトルロゴ.png"), false);
	lpMap.stages = STAGE1;
	return true;
}

void GameInit(void)
{
	cnt = 0;
	lpEnemy.Init();
	lpPlayer.Init();
	lpMap.Init();
	if (centerSpawn == true)
	{
		lpPlayer.pos = { 62*CHIP_SIZE,SCREEN_SIZE_Y - 6 * CHIP_SIZE };
		lpMap.centerFlg = true;
	}
	playerstock = lpPlayer.playerStock;
}

void GameStock(void)
{
	GameStockDraw();
}

void GameStockDraw(void)
{
	DrawGraph(SCREEN_SIZE_X / 2 - CHIP_SIZE, SCREEN_SIZE_Y/2, lpPlayer.pGraph[0], true);
	DrawFormatString(SCREEN_SIZE_X / 2, SCREEN_SIZE_Y / 2 + 16, 0xffffff, _T("×%d"), playerstock);
	SetFontSize(40);
}

void GameTitle(void)
{
	GameTitleDraw();
}

void GameTitleDraw(void)
{
	//DrawString(0, 0, _T("GameTitle"), 0xFFFFFF);

	for (int y = 0; y < SCREEN_SIZE_Y / CHIP_SIZE; y++)
	{
		for (int x = 0; x < SCREEN_SIZE_X / CHIP_SIZE; x++)
		{
			if (titleBack[y][x] == 9)DrawGraph(x*CHIP_SIZE, y*CHIP_SIZE, lpMap.back[0], false);

			switch (titleMap[y][x])
			{
			case 1:
				DrawGraph(x*CHIP_SIZE, y*CHIP_SIZE, lpMap.grass, true);
				break;
			case 3:
				DrawGraph(x*CHIP_SIZE, y*CHIP_SIZE, lpMap.blocks[4], false);
				break;
			case 4:
				DrawGraph(x*CHIP_SIZE, y*CHIP_SIZE, lpMap.mountain[0], true);
				break;
			case 5:
				DrawGraph(x*CHIP_SIZE, y*CHIP_SIZE, lpMap.mountain[1], true);
				break;
			case 6:
				DrawGraph(x*CHIP_SIZE, y*CHIP_SIZE, lpMap.mountain[2], true);
				break;
			case 7:
				DrawGraph(x*CHIP_SIZE, y*CHIP_SIZE, lpMap.mountain[3], true);
				break;
			case 8:
				DrawGraph(x*CHIP_SIZE, y*CHIP_SIZE, lpMap.mountain[4], true);
				break;
			case 9:
				DrawGraph(x*CHIP_SIZE, y*CHIP_SIZE, lpMap.mountain[5], true);
				break;
			default:
				break;
			}
		}
	}
	DrawGraph(200, SCREEN_SIZE_Y - 128, lpPlayer.pGraph[0], true);
	DrawGraph(SCREEN_SIZE_X / 8, SCREEN_SIZE_Y / 6, title, true);
	SetFontSize(30);
	DrawString(SCREEN_SIZE_X / 3, SCREEN_SIZE_Y - 256, _T("スペースキーを押せ！"), 0x000000);
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
		lpMap.Update();
		lpPlayer.Update();
		lpEnemy.Update();
		if (lpMap.centerFlg == true)centerSpawn = true;
		//if (trgKey[START]) gamemode = GMODE_GAMEOVER;
	}
	GameMainDraw();
	if (pause)
	{
		SetDrawBright(255, 255, 255);
		//DrawString(360, 292, _T("PAUSE"), 0xFFFFFF);
	}
}

void GameMainDraw(void)
{
	lpMap.Draw();
	lpPlayer.Draw();
	lpEnemy.Draw();
	//DrawFormatString(0, 0, 0xFFFFFF, _T("GameMain:%d"), gameCounter);
}

void PlayerDeath(void)
{
	stockCnt = 0;
	if (cnt == 0)
	{
		lpPlayer.playerStock--;
		PlaySoundMem(lpPlayer.sound2, DX_PLAYTYPE_BACK);
	}
	cnt++;
	if (cnt > 115)gamemode = GMODE_INIT;
}