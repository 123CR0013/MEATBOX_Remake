#pragma once
#include"appframe.h"

class SettingScreen :public UIScreen
{
public:
	SettingScreen(class ModeUI* owner,class MoveUI* select,const Vector2& backPos);
	virtual ~SettingScreen();

	void Update()override;

private:
	unsigned int _buttonNum;
	std::array<class Graph*, 3>_buttons;
	std::array<class Graph*, 5>_BGMLevelUIs;
	std::array<class Graph*, 5>_SELevelUIs;

	unsigned int _BGMLevel;
	unsigned int _SELevel;

	class MoveUI* _select;
	const Vector2 _backPos;
};