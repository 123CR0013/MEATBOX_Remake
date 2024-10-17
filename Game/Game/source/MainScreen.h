#pragma once
#include"appframe.h"

class MainScreen :public UIScreen
{
public:
	MainScreen(class ModeGame* owner);
	~MainScreen();

	void Update()override;

	void SetStepCount(UINT time) { _stepCount = time; }
	void SetStageNum(UCHAR stageNum) { _stageNum = stageNum; }
private:
	Graph* _counterFrameLeft;
	Graph* _counterFrameRight;
	Graph* _stageCounter;
	class Number* _stepCounter;
	UINT _stepCount;
	UCHAR _stageNum;


	//デバッグ
	int frameCount;
};