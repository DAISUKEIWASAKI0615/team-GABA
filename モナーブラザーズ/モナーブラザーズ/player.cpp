#include "player.h"
#include "DxLib.h"

player::player()
{
	pos = { 200,100 };
	LoadDivGraph(_T("‰æ‘œ/ƒ‚ƒi[.png"), 3, 1, 3, 64, 64,pGraph,false);
}

player::~player()
{
}

void player::Draw()
{
	DrawGraph(pos.x, pos.y, pGraph[0], true);
}

void player::Update()
{
}
