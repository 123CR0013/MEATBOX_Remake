#pragma once
#include"appframe.h"


class ResultScreen final :public UIScreen
{
public:
	ResultScreen(class ModeGame* owner);
	~ResultScreen();

	void Update()override;
	void Draw()override;

private:
	Graph* _mainFrame;				//ƒŠƒUƒ‹ƒgŒ‹‰Ê‚Ì”wŒi

	Graph* _itemStepCounter;
	class Number* _stepCounter;
	UINT _stepCount;

	Graph* _itemKillCounter;
	class Number* _killCounter;
	UINT _killCount;

	Graph* _gaugeFrame;
	Graph* _gaugeBar;
	Clip* _gaugeClip;

	Graph* _rank;

	Graph* _subFrame;				//‘I‘ðŽˆ‚Ì”wŒi
	std::array<Button*, 3>_buttons;
	UINT _selectNum;

	UINT _frameCount;

	enum class Step
	{
		kNone,
		kMainFrameAnimation,
		kStepCount,
		kKillCount,
		kGauge,
		kRank,
		kChoice
	}_step;

	const ModeBase::ResultData _resultData;
	const int _worldID;
	const int _stageID;
	int _worldIDNext;
	int _stageIDNext;

	class ModeGame* _game;
};