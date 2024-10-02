#pragma once
#include"UI.h"
#include"../ResourceServer/ResourceServer.h"

class FlipBook :public UI {
public:
	FlipBook(class UIScreen* owner,unsigned int order)
		:UI(owner,order) 
	{

	}

	virtual ~FlipBook(){}

	void Draw()override final {
		auto _pos = GetVertexes();
		DrawModiGraph(
			_pos[0].x, _pos[0].y,
			_pos[1].x, _pos[1].y,
			_pos[2].x, _pos[2].y,
			_pos[3].x, _pos[3].y,
			_handles[_framCount++ / _flipSpeed % static_cast<unsigned int>(_handles.size())], TRUE
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
			FileName,AllNum,
			XNum,YNum,
			XSize,YSize,
			_handles.data()
		) > 0;
	}

	void SetFlipSpeed(unsigned int flipSpeed) { _flipSpeed = flipSpeed; }
private:
	std::vector<int>_handles;	
	unsigned int _flipSpeed;	//フリップするフレームの間隔
	unsigned int _framCount;	
};