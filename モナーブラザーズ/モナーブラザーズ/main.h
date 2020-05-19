#pragma once

#define SCREEN_SIZE_X	1024
#define SCREEN_SIZE_Y	768

#define CHIP_SIZE        64									// 一つのチップのサイズ
#define MAP_WIDTH        (SCREEN_SIZE_X / CHIP_SIZE)*4		// マップの横幅
#define MAP_HEIGHT       SCREEN_SIZE_Y / CHIP_SIZE			// マップの縦幅
#define P1 3.141592

typedef struct {
	float x;
	float y;
}XY;

typedef struct {
	int x;
	int y;
}xy;
