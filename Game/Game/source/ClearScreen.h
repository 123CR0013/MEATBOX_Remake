#pragma once
#include"appframe.h"

class ClearScreen final:public UIScreen
{
public:
	ClearScreen(class ModeUI* owner);
	~ClearScreen();

	void Update()override;

private:
	Graph* _backGround;
};