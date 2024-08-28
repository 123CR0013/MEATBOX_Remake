#pragma once
#include"UI.h"

class Box :public UI {
public:
	Box(class UIScreen* owner,unsigned int order = 100)
		:UI(owner,order) 
		,_fillFlag(false)
		,_color(GetColor(125,125,125))
	{

	}
	virtual ~Box(){}

	void Draw()override final {
		// 塗りつぶしか？
		auto _pos = GetVertexes();
		if (_fillFlag) {
			// 変換した座標で、線で四角を描画する
			DrawLine(
				static_cast<int>(_pos[0].x),static_cast<int> (_pos[0].y),	// 左上
				static_cast<int>(_pos[1].x),static_cast<int> (_pos[1].y),	// 右上
				_color);
			DrawLine(
				static_cast<int>(_pos[1].x),static_cast<int> (_pos[1].y),	// 右上
				static_cast<int>(_pos[2].x),static_cast<int> (_pos[2].y),	// 右下
				_color);
			DrawLine(
				static_cast<int>(_pos[2].x),static_cast<int> (_pos[2].y),	// 右下
				static_cast<int>(_pos[3].x),static_cast<int> (_pos[3].y),	// 左下
				_color);
			DrawLine(
				static_cast<int>(_pos[3].x),static_cast<int> (_pos[3].y),	// 左下
				static_cast<int>(_pos[0].x),static_cast<int> (_pos[0].y),	// 左上
				_color);
		}
		else {
			DrawTriangle(
				static_cast<int>(_pos[0].x) , static_cast<int>(_pos[0].y),	// 左上
				static_cast<int>(_pos[1].x) , static_cast<int>(_pos[1].y),	// 右上
				static_cast<int>(_pos[3].x) , static_cast<int>(_pos[3].y),	// 左下
				_color, TRUE
			);

			DrawTriangle(
				static_cast<int>(_pos[1].x) , static_cast<int>(_pos[1].y) ,	// 右上
				static_cast<int>(_pos[3].x) , static_cast<int>(_pos[3].y) ,	// 左下
				static_cast<int>(_pos[2].x) , static_cast<int>(_pos[2].y) ,	// 右下
				_color, TRUE
			);
		}
	}

	void SetFillFlag(bool fillFlag) { _fillFlag = fillFlag; }
	void SetColor(unsigned int r, unsigned int g, unsigned int b) { _color = GetColor(r, g, b); }

private:
	// 塗りつぶしフラグ
	bool _fillFlag;
	unsigned int _color;
};