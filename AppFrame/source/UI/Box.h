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
		// �h��Ԃ����H
		auto _pos = GetVertexes();
		if (_fillFlag) {
			// �ϊ��������W�ŁA���Ŏl�p��`�悷��
			DrawLine(
				static_cast<int>(_pos[0].x),static_cast<int> (_pos[0].y),	// ����
				static_cast<int>(_pos[1].x),static_cast<int> (_pos[1].y),	// �E��
				_color);
			DrawLine(
				static_cast<int>(_pos[1].x),static_cast<int> (_pos[1].y),	// �E��
				static_cast<int>(_pos[2].x),static_cast<int> (_pos[2].y),	// �E��
				_color);
			DrawLine(
				static_cast<int>(_pos[2].x),static_cast<int> (_pos[2].y),	// �E��
				static_cast<int>(_pos[3].x),static_cast<int> (_pos[3].y),	// ����
				_color);
			DrawLine(
				static_cast<int>(_pos[3].x),static_cast<int> (_pos[3].y),	// ����
				static_cast<int>(_pos[0].x),static_cast<int> (_pos[0].y),	// ����
				_color);
		}
		else {
			DrawTriangle(
				static_cast<int>(_pos[0].x) , static_cast<int>(_pos[0].y),	// ����
				static_cast<int>(_pos[1].x) , static_cast<int>(_pos[1].y),	// �E��
				static_cast<int>(_pos[3].x) , static_cast<int>(_pos[3].y),	// ����
				_color, TRUE
			);

			DrawTriangle(
				static_cast<int>(_pos[1].x) , static_cast<int>(_pos[1].y) ,	// �E��
				static_cast<int>(_pos[3].x) , static_cast<int>(_pos[3].y) ,	// ����
				static_cast<int>(_pos[2].x) , static_cast<int>(_pos[2].y) ,	// �E��
				_color, TRUE
			);
		}
	}

	void SetFillFlag(bool fillFlag) { _fillFlag = fillFlag; }
	void SetColor(unsigned int r, unsigned int g, unsigned int b) { _color = GetColor(r, g, b); }

private:
	// �h��Ԃ��t���O
	bool _fillFlag;
	unsigned int _color;
};