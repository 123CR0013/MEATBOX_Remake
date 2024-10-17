#pragma once
#include"appframe.h"

class MenuScreen :public UIScreen
{
public:
	MenuScreen(class ModeUI* owner);
	virtual ~MenuScreen();

	void Update()override;
	void Draw()override;
private:
	unsigned int _buttonNum;
	class Graph* _backGround;
	std::array<class Graph*,3>_buttons;

	class MoveUI* _carsol;

	bool _isFinish;
};