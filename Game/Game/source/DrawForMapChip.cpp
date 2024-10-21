#include "DrawForMapChip.h"
#include "Map.h"
#include "ApplicationMain.h"


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

void DrawForMapChip::MyDrawModiGraphWithScreenSize(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle, int turn)
{
	VECTOR pos = { position.x * CHIP_W + CHIP_W / 2.0f, position.y * CHIP_H + CHIP_H / 2.0f, 0 };
	float rate = g_oApplicationMain.DispSizeW() / 1920.0f;
	pos.x *= rate;
	pos.y *= rate;
	MyDraw::MyDrawModiGraph(mView, pos, zoom, angle, width * rate, height * rate, cgHandle, turn);
}

void DrawForMapChip::MyDrawModiGraphWithScreenSize(MATRIX mView, Vector3 position, float zoom, float angle, int width, int height, int cgHandle, int turn)
{
	Vector3 pos = { position.x * CHIP_W + CHIP_W / 2.0f, position.y * CHIP_H + CHIP_H / 2.0f, 0 };
	float rate = g_oApplicationMain.DispSizeW() / 1920.0f;
	pos.x *= rate;
	pos.y *= rate;
	MyDraw::MyDrawModiGraph(mView, pos.ToVECTOR(), zoom, angle, width * rate, height * rate, cgHandle, turn);
}
