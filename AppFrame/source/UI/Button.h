#pragma once
#include"UI.h"
#include<functional>

class Button :public UI {
public:
	Button(class UIScreen* owner,unsigned int order= 100)
		:UI(owner,order)
		, _color(GetColor(125, 125, 125))
	{

	}
	virtual ~Button(){}

	void Draw()override final {

		auto _pos = GetVertexes();
		DrawTriangle(
			static_cast<int>(_pos[0].x), static_cast<int>(_pos[0].y),	// 左上
			static_cast<int>(_pos[1].x), static_cast<int>(_pos[1].y),	// 右上
			static_cast<int>(_pos[3].x), static_cast<int>(_pos[3].y),	// 左下
			_color, TRUE
		);

		DrawTriangle(
			static_cast<int>(_pos[1].x), static_cast<int>(_pos[1].y),	// 右上
			static_cast<int>(_pos[3].x), static_cast<int>(_pos[3].y),	// 左下
			static_cast<int>(_pos[2].x), static_cast<int>(_pos[2].y),	// 右下
			_color, TRUE
		);
	}

	void SetColor(unsigned int r, unsigned int g, unsigned int b) {
		_color = GetColor(r, g, b);
	}

	void SetExe(std::function<void()>exe) { _exe = exe; }
	void Push() { _exe(); }
private:
	unsigned int _color;

	std::function<void()>_exe;
};