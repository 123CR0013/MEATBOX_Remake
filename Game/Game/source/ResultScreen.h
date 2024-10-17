#pragma once
#include"appframe.h"


class ResultScreen final :public UIScreen
{
public:
	ResultScreen(class ModeUI* owner);
	~ResultScreen();

	void Update()override;
	void Draw()override;

private:
	Graph* _mainFrame;				//���U���g���ʂ̔w�i

	Graph* _itemStepCounter;
	class Number* _stepCounter;
	UINT _stepCount;

	Graph* _itemKillCounter;
	class Number* _killCounter;
	UINT _killCount;

	Graph* _gaugeFrame;
	Graph* _gaugeBar;
	Clip* _gaugeClip;

	Graph* _subFrame;				//�I�����̔w�i
	std::array<Graph*, 3>_buttons;
	UINT _selectNum;

	UINT _frameCount;

	enum class Step
	{
		kNone,
		kMainFrameAnimation,
		kStepCount,
		kKillCount,
		kGauge,
		kChoice
	}_step;


};