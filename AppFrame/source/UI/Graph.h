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
		auto _pos = GetVertexes();

		DrawModiGraphF(
			_pos[0].x, _pos[0].y,
			_pos[1].x, _pos[1].y,
			_pos[2].x, _pos[2].y,
			_pos[3].x, _pos[3].y,
			_handle, TRUE
		);
	}

	bool Load(const char* fileName) {
		return ResourceServer::LoadGraph(fileName);
	}
private:
	int _handle;
};