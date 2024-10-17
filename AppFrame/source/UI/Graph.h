#pragma once
#include"UI.h"
#include"../ResourceServer/ResourceServer.h"

class Graph :public UI 
{
public:
	Graph(class UIScreen* owner,unsigned int order = 100)
		:UI(owner,order)
		,_handle(-1)
	{
		
	}

	virtual ~Graph(){}

	void Draw()override final {
		if (_handle == -1)return;

		auto _pos = GetVertexes();

		DrawModiGraphF(
			_pos[0].x, _pos[0].y,
			_pos[1].x, _pos[1].y,
			_pos[2].x, _pos[2].y,
			_pos[3].x, _pos[3].y,
			_handle, TRUE
		);
	}

	void LoadSize() {
		if (_handle != -1)
		{
			int width, height;
			GetGraphSize(_handle, &width, &height);
			_width = static_cast<float>(width);
			_height = static_cast<float>(height);
		}
	}

	bool Load(const std::string& fileName) {
		_handle = ResourceServer::LoadGraph(fileName);
		LoadSize();
		return _handle != -1;
	}

	int GetHandle()const { return _handle; }
	void SetHandle(int handle) { _handle = handle; LoadSize(); }
private:
	int _handle;
};