#include "base.h"
#include "DxLib.h"


base::base()
{
	pos = { 0,0 };
	graph = 0;
	speed = 0;
}

base::~base()
{
}

void base::Draw()
{
	DrawGraph(pos.x, pos.y, graph, true);
}

void base::FloorColision(VECTOR pos, int mapdate[MAP_HEIGHT][MAP_WIDTH])
{
	//return mapdate[pos.y / CHIP_SIZE][pos.x / CHIP_SIZE];
}
