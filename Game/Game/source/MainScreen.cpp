#include"MainScreen.h"
#include"ModeGame.h"
#include"Number.h"

MainScreen::MainScreen(ModeGame* owner)
	:UIScreen(owner)
	, _stepCounter(NEW Number(this, "res/UI/Game/number/ui_ingame_stepcount_0", 1000))
	,_stepCount(100)
	,_stageNum(0)
	,frameCount(0)
{
	_counterFrameLeft = NEW Graph(this);
	_counterFrameLeft->Load("res/UI/Game/ui_ingame_countframe_01.png");
	_counterFrameLeft->SetLocation(325.f,715.f);

	_counterFrameRight = NEW Graph(this);
	_counterFrameRight->Load("res/UI/Game/ui_ingame_countframe_02.png");
	_counterFrameRight->SetLocation(1595.f,715.f);

	{
		_stageCounter = NEW Graph(this, 1000);
		std::string fileName = "ui_ingame_stagecount_0" + std::to_string(_stageNum + 1) + ".png";
		_stageCounter->Load("res/UI/Game/stagenumber/" + fileName);
		_stageCounter->SetLocation(1695.f,860.f);
	}
	_stepCounter->SetNumberWidth(100.f);
	_stepCounter->SetNumberHeight(100.f);
	_stepCounter->SetLocation(225.f, 860.f);
	_stepCounter->SetNumber(_stepCount);

	//‘€ìà–¾UI
	{
		_tutorialMove = NEW Graph(this);
		_tutorialMove->Load("res/UI/ui_tutorial_move_01.png");
		_tutorialMove->SetLeftLocation(30.f, 15.f);

		_tutorialRetry = NEW Graph(this);
		_tutorialRetry->Load("res/UI/ui_tutorial_retry_01.png");
		_tutorialRetry->SetLeftLocation(1558.f, 15.f);

		_tutorialMenu = NEW Graph(this);
		_tutorialMenu->Load("res/UI/ui_tutorial_menu_01.png");
		_tutorialMenu->SetLeftLocation(746.f, 997.f);
	}
}

MainScreen::~MainScreen(){}

void MainScreen::Update()
{
	UIScreen::Update();

	if (global._key & PAD_INPUT_RIGHT)
	{
		if (frameCount++ == 3)
		{
			_stepCounter->SetNumber(++_stepCount);
			frameCount = 0;
		}
	}

	else if (global._key & PAD_INPUT_LEFT)
	{
		if (frameCount++ == 3)
		{
			_stepCounter->SetNumber(--_stepCount);
			frameCount = 0;
		}
	}

	else
	{
		frameCount = 0;
	}
}