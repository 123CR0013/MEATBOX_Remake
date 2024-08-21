#pragma once
#include "appframe.h"

class DrawForMapChip
{
public:
	// マップチップ単位での描画
	// @param mView ビュー行列
	// @param position マップチップ座標
	// @param zoom 拡大率
	// @param angle 回転角度
	// @param width 描画サイズ 幅
	// @param height 描画サイズ 高さ
	// @param cgHandle 画像ハンドル
	// @param turn 画像の向き 0:そのまま 0以外:左右反転
	static void MyDrawModiGraph(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle, int turn = 0);
	static void MyDrawModiGraph(MATRIX mView, Vector3 position, float zoom, float angle, int width, int height, int cgHandle, int turn = 0);
};