#pragma once
#include"appframe.h"

class MenuScreen :public UIScreen
{
public:
	MenuScreen(class ModeUI* owner);
	virtual ~MenuScreen();

	void Update()override;

private:
	unsigned int _buttonNum;
	std::array<class Graph*,3>_buttons;

	class MoveUI* _select;
};