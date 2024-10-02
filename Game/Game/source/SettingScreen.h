#pragma once
#include"appframe.h"

class SettingScreen :public UIScreen
{
public:
	SettingScreen(class ModeUI* owner);
	virtual ~SettingScreen();

	void Update()override;

private:
	unsigned int _buttonNum;
	class Graph* _backGround;
	std::array<class Graph*, 3>_buttons;
	std::array<class Graph*, 5>_BGMLevelUIs;
	std::array<class Graph*, 5>_SELevelUIs;

	unsigned int _BGMLevel;
	unsigned int _SELevel;

	class MoveUI* _select;

	bool _isFinish;
};