#pragma once
#include"../../../AppFrame/source/UI/Empty.h"
#include"MoveUI.h"
#include<array>

constexpr int BUTTON_NUM = 5;

class StageSelectButton :public Empty {
public:
	StageSelectButton(class UIScreen* owner, size_t stageNum);
	virtual ~StageSelectButton();

	Graph* GetMonitor()const { return _monitor; }

	std::array<class Graph*, BUTTON_NUM> GetButtons()& { return _buttons; }
	std::array<bool, BUTTON_NUM>& GetStageDatas()& { return _stageDatas; }
private:
	unsigned int _stageNum;

	class Graph* _backGround;
	class Graph* _monitor;
	std::array<class Graph*, BUTTON_NUM>_buttons;
	std::array<bool, BUTTON_NUM>_stageDatas;

};