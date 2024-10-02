#pragma once
#include"appframe.h"

class MoveUI :public Graph
{
public:
	MoveUI(class UIScreen* owner):Graph(owner),_frameCount(0.f),_takeFrame(0.f){}
	virtual ~MoveUI(){}

	void Process()override
	{
		Graph::Process();
		if (_frameCount < _takeFrame)
		{
			_frameCount++;

			_transform.mLocation = Vector2::Lerp(_from, _to, _frameCount / _takeFrame);
		}
		else
		{
			_from = _transform.mLocation;
		}
	}

	void SetFrom(const Vector2& from) { _from = from; }
	void SetTo(const Vector2& to) { _to = to; }
	void SetFrameCount(float frameCount) { _frameCount = frameCount; }
	void SetTakeFrame(float takeFrame) { _frameCount = _takeFrame = takeFrame; }
private:
	Vector2 _from;
	Vector2 _to;
	float _frameCount;		//経過したフレーム数
	float _takeFrame;		//イージングにかからフレーム数
};