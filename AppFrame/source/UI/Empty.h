#pragma once
#include"UI.h"
class Empty :public UI {
public:
	Empty(class UIScreen* owner,unsigned int order = 100)
		:UI(owner,order)
	{

	}

	virtual ~Empty(){}

	virtual void Draw()override{}

};