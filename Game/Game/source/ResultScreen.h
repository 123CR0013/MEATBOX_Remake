#pragma once
#include"appframe.h"


class ResultScreen final :public UIScreen
{
public:
	ResultScreen(class ModeUI* owner);
	~ResultScreen();

	void Update()override;

private:
	int _score;
	Text* _scoreText;
	int _eleNum;
	int _frameCount;
};