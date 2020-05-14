#pragma once

#define SCREEN_SIZE_X 1024
#define SCREEN_SIZE_Y 768

#define CHIP_SIZE        (64)					         // ��̃`�b�v�̃T�C�Y
#define MAP_WIDTH        (SCREEN_SIZE_X / CHIP_SIZE)*2     // �}�b�v�̉���
#define MAP_HEIGHT       (SCREEN_SIZE_Y / CHIP_SIZE)     // �}�b�v�̏c��
#define MOVE_FRAME      15          // �ړ��ɂ�����t���[����
#define P1 3.141592

typedef struct {
	float x;
	float y;
}XY;

typedef struct {
	int x;
	int y;
}xy;
