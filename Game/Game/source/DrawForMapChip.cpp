#include "DrawForMapChip.h"
#include "Map.h"


void DrawForMapChip::MyDrawModiGraph(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle, int turn)
{
	VECTOR pos = VGet(position.x * CHIP_W + CHIP_W / 2.0f, position.y * CHIP_H + CHIP_H / 2.0f, 0);
	MyDraw::MyDrawModiGraph(mView, pos, zoom, angle, width, height, cgHandle, turn);
}

void DrawForMapChip::MyDrawModiGraph(MATRIX mView, Vector3 position, float zoom, float angle, int width, int height, int cgHandle, int turn)
{
	Vector3 pos = { position.x * CHIP_W + CHIP_W / 2.0f, position.y * CHIP_H + CHIP_H / 2.0f, 0 };
	MyDraw::MyDrawModiGraph(mView, pos.ToVECTOR(), zoom, angle, width, height, cgHandle, turn);
}
