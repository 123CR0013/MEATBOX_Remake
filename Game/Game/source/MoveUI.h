#pragma once
#include"appframe.h"

class MoveUI :public Box
{
public:
	MoveUI(class UIScreen* owner):Box(owner),_frameCount(0.f),_takeFrame(0.f){}
	virtual ~MoveUI(){}

	void Process()override
	{
		Box::Process();
		if (_frameCount > 0.f)
		{
			_frameCount -= 1.f;

			_transform.mLocation = Vector2::Lerp(_from, _to, 1.f - _frameCount / _takeFrame);
		}
		else
		{
			_from = _transform.mLocation;
		}
	}

	void SetFrom(const Vector2& from) { _from = from; }
	void SetTo(const Vector2& to) { _to = to; }
	void SetFrameCount(float frameCount) { _frameCount = frameCount; }
	void SetTakeFrame(float takeFrame) { _takeFrame = takeFrame; }
private:
	Vector2 _from;
	Vector2 _to;
	float _frameCount;
	float _takeFrame;
};