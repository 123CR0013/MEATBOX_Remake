#pragma once
#include"UI.h"
#include"../ResourceServer/ResourceServer.h"

class FlipBook :public UI {
public:
	FlipBook(class UIScreen* owner, unsigned int order)
		:UI(owner, order)
		, _flipSpeed(60)
		, _framCount(0)
		, _clockWise(1)
	{

	}

	virtual ~FlipBook() {}

	void Draw()override final {
		auto _pos = GetVertexes();
		DrawModiGraphF(
			_pos[0].x, _pos[0].y,
			_pos[1].x, _pos[1].y,
			_pos[2].x, _pos[2].y,
			_pos[3].x, _pos[3].y,
			_handles[(_framCount += _clockWise) / _flipSpeed % static_cast<unsigned int>(_handles.size())], TRUE
		);
	}

	bool LoadDiv(const TCHAR* FileName, int AllNum,
		int XNum, int YNum,
		int XSize, int YSize)
	{
		_handles.resize(AllNum);

		_width = XSize;
		_height = YSize;

		return ResourceServer::LoadDivGraph(
			FileName, AllNum,
			XNum, YNum,
			XSize, YSize,
			_handles.data()
		) > 0;
	}

	void Play() { _clockWise = 1; }
	void Reverse() { _clockWise = -1; }
	void Stop() { _clockWise = 0; }

	void Reset() { _framCount = 0; }

	void SetFlipSpeed(unsigned int flipSpeed) { _flipSpeed = flipSpeed; }
private:
	std::vector<int>_handles;
	unsigned int _flipSpeed;	//フリップするフレームの間隔
	unsigned int _framCount;
	char _clockWise;	//時計回りかどうか
};