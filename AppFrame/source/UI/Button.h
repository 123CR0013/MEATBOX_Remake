#pragma once
#include"UI.h"

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
			static_cast<int>(_pos[0].x), static_cast<int>(_pos[0].y),	// ç∂è„
			static_cast<int>(_pos[1].x), static_cast<int>(_pos[1].y),	// âEè„
			static_cast<int>(_pos[3].x), static_cast<int>(_pos[3].y),	// ç∂â∫
			_color, TRUE
		);

		DrawTriangle(
			static_cast<int>(_pos[1].x), static_cast<int>(_pos[1].y),	// âEè„
			static_cast<int>(_pos[3].x), static_cast<int>(_pos[3].y),	// ç∂â∫
			static_cast<int>(_pos[2].x), static_cast<int>(_pos[2].y),	// âEâ∫
			_color, TRUE
		);
	}

	void SetColor(unsigned int r, unsigned int g, unsigned int b) {
		_color = GetColor(r, g, b);
	}
private:
	unsigned int _color;
};