#pragma once
#include"../../../AppFrame/source/UI/Empty.h"
#include<array>

constexpr int BUTTON_NUM = 3;

class StageSelectButton :public Empty {
public:
	StageSelectButton(class UIScreen* owner, size_t stageNum);
	virtual ~StageSelectButton();

	std::array<class Button*, BUTTON_NUM> GetButtons()& { return _buttons; }
	std::array<bool, BUTTON_NUM>& GetStageDatas()& { return _stageDatas; }
private:
	unsigned int _stageNum;

	class Text* _stageName;
	class Box* _backGround;
	std::array<class Button*, BUTTON_NUM>_buttons;
	std::array<bool, BUTTON_NUM>_stageDatas;

};