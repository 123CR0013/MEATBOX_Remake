#pragma once
#include "appframe.h"
#include <vector>

class AnimationInfo
{
public:
	AnimationInfo();
	~AnimationInfo();

	// 画像ハンドル
	std::vector<int> _graphHandle;
	// 描画順テーブル
	std::vector<int> _drawTbl;
	// テーブルの要素数
	int _tblNum;
	// 1枚あたりの表示フレーム数
	int _framePerSheet;
};