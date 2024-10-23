#pragma once
#include"appframe.h"

class TitleScreen final:public UIScreen
{
public:
	TitleScreen(class ModeUI* owner);
	~TitleScreen();

	void Update()override;


private:
	Graph* _backGround;
	Box* _fadeBox;
};