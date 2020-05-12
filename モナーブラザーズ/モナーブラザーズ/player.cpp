#include "player.h"
#include "DxLib.h"
#include "keycheck.h"

player::player()
{
	pos = { 200,100 };
	LoadDivGraph(_T("âÊëú/ÉÇÉiÅ[.png"), 3, 3, 1, 64, 64,pGraph,false);
	speed = 5;
}

player::~player()
{
}

void player::Draw()
{
	if (newKey[P1_LEFT])
	{
		DrawTurnGraph(pos.x, pos.y, pGraph[0], true);
	}
	else
	{
		DrawGraph(pos.x, pos.y, pGraph[0], true);

	}
}

void player::Update()
{
	if (newKey[P1_UP])
	{
		pos.y -= speed*5;
	}
	//if (newKey[P1_DOWN])
	//{
	//	pos.y += speed;
	//}
	if (newKey[P1_LEFT])
	{
		pos.x -= speed;
	}
	if (newKey[P1_RIGHT])
	{
		pos.x += speed;
	}
	if (pos.y + 64 < 500)
	{
		pos.y+=5;
	}
	
}
