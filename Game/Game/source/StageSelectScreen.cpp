#include"StageSelectScreen.h"
#include"StageSelectButton.h"
#include"MenuScreen.h"

constexpr float TAKE_FRAME = 30.f;

StageSelectScreen::StageSelectScreen(ModeUI* owner)
	:UIScreen(owner)
	,_frameCount(0)
	,_takeFrame(TAKE_FRAME)
	,_fromFrame(0)
	,_isSelect(false)
	,_stageTypeNum(0)
	,_stageNum(0)
	,_moveArrowX(0)
{
	for (size_t i = 0;i<_selectButtons.size();++i) 
	{
		_selectButtons[i] = NEW StageSelectButton(this,i);
	}

	_pos = _selectButtons.front()->GetLocation();
	_from = _pos;

	_containerParent = NEW Empty(this,500);

	_container = NEW Graph(this,1000);
	_container->Load("res/UI/Select/Stage/ui_stage_container_01.png");
	_container->RegistParent(_containerParent);
	_container->SetLocation(0.f, -_container->GetHeight() / 2.f);

	_containerParent->SetLocation(1670.f, 700.f + _container->GetHeight() / 2.f);

	_meatBox = NEW Graph(this,500);
	_meatBox->Load("res/UI/Select/Stage/ui_stage_boss_01.png");
	_meatBox->SetLocation(0.f, -_container->GetHeight() / 2.f);
	_meatBox->RegistParent(_containerParent);

	_terminal = NEW Graph(this,1000);
	_terminal->Load("res/UI/Select/Stage/ui_stage_terminal_01.png");
	_terminal->SetLocation(966.f, 834);

	_light = NEW FlipBook(this, 1000);
	_light->LoadDiv("res/UI/Select/Stage/ui_stage_light_01_sheet.png", 15, 15, 1, 10620 / 15, 218);
	_light->SetLocation(966.f, 19.f);
	_light->Stop();

	_armParent = NEW Empty(this, 1000);

	_arm = NEW FlipBook(this, 1000);
	_arm->LoadDiv("res/UI/Select/Stage/ui_stage_arm_01_sheet.png", 3, 3, 1, 1455 / 3, 1080);
	_arm->SetLocation(0.f, _arm->GetHeight() / 2.f);
	_arm->Stop();
	_arm->RegistParent(_armParent);
	
	_armParent->SetLocation(245.f, 263.f - _arm->GetHeight() / 2.f);

	{
		float swingWidth = 10.f;
		CreateRotateAnim("sway", MyMath::DegToRad(swingWidth), 5.f);
		int i;
		for (i = 1; i <= 10; ++i)
		{
			float sw = swingWidth;
			swingWidth -= 1.f;
			sw += swingWidth;
			sw *= i % 2 == 1 ? -1.f : 1.f;
			CreateRotateAnim("sway", MyMath::DegToRad(sw), 5);
		}
	}
	CreateWorldRotateAnim("waving_left", -PI / 9.f, 20,Easing::OUT_CUBIC);
	CreateWorldRotateAnim("waving_right", PI / 9.f, 20,Easing::OUT_CUBIC);
	CreateWorldRotateAnim("waving_back",0.f, 20, Easing::OUT_CUBIC);

	CreateScaleAnim("Select", 0.1f, 0.1f, 5,Easing::OUT_QUART);
	CreateScaleAnim("Monitor", 1.f, 1.f, 10,Easing::OUT_QUART);
	{
		Vector2 monitorPos(966.f, 482.f);
		auto ptr = _selectButtons.front()->GetMonitor();
		auto ptrPos = ptr->GetWorldLocation();
		monitorPos = monitorPos * Matrix3::CreateTranslation(ptr->GetWorldLocation()).Invert();

		CreateLocationAnim("Monitor", monitorPos.x, monitorPos.y , 10);
	}
}

StageSelectScreen::~StageSelectScreen(){}

void StageSelectScreen::Update() {
	UIScreen::Update();

	if (_isSelect)
	{
		if (global._trg & PAD_INPUT_3)
		{
			std::function<void()>exe = [=]()mutable 
				{
					_selectButtons[_stageTypeNum]->GetMonitor()->SetDrawOrder(500);
					for (auto&& button : _selectButtons[_stageTypeNum]->GetButtons())
					{
						button->SetDrawOrder(501);
					}
				};
			ModeTimeTable::Add(exe, 20);

			_selectButtons[_stageTypeNum]->GetMonitor()->ReverseAnimation();
			_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->ReverseAnimation();
			_stageNum = 0;
			_isSelect = false;
		}

		for (size_t i = 0; i < _selectButtons[_stageTypeNum]->GetButtons().size(); ++i)
		{
			//if (_selectButtons[_stageTypeNum]->GetButtons()[i]->IsRightClickTrg())
			//{
			//	if (!_selectButtons[_stageTypeNum]->GetStageDatas()[i])
			//	{
			//		_selectButtons[_stageTypeNum]->GetStageDatas()[i] = true;
			//		_selectButtons[_stageTypeNum]->GetButtons()[i]->SetColor(0, 255, 255);
			//	}
			//	else if(i != _stageNum)
			//	{
			//		_selectButtons[_stageTypeNum]->GetStageDatas()[i] = false;
			//		_selectButtons[_stageTypeNum]->GetButtons()[i]->SetColor(255, 255, 0);
			//	}
			//}
		}

		if (global._trg & PAD_INPUT_LEFT)
		{
			if (0 < _stageNum) {
				if (_selectButtons[_stageTypeNum]->GetStageDatas()[_stageNum - 1])
				{
					_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->ReverseAnimation();
					_stageNum--;
					_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->PlayAnimation("Select");
				}
			}
		}

		if (global._trg & PAD_INPUT_RIGHT )
		{
			if (_stageNum < _selectButtons[_stageTypeNum]->GetButtons().size() - 1) {
				if (_selectButtons[_stageTypeNum]->GetStageDatas()[_stageNum + 1])
				{
					_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->ReverseAnimation();
					_stageNum++;
					_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->PlayAnimation("Select");
				}
			}
		}
	}
	else
	{
		

		if (global._trg & PAD_INPUT_LEFT && _stageTypeNum > 0)
		{
			_stageTypeNum = _stageTypeNum - 1;
			_to = _selectButtons[_stageTypeNum]->GetLocation();
			_frameCount = _moveArrowX == 1 ? TAKE_FRAME / 2.f : TAKE_FRAME;
			_takeFrame = _frameCount;
			_armParent->PlayAnimation("waving_left");
			_moveArrowX = -1;
		}

		if (global._trg & PAD_INPUT_RIGHT && _stageTypeNum < _selectButtons.size() - 1)
		{
			_stageTypeNum = _stageTypeNum + 1;
			_to = _selectButtons[_stageTypeNum]->GetLocation();
			_frameCount = _moveArrowX == -1 ? TAKE_FRAME / 2.f: TAKE_FRAME;
			_takeFrame = _frameCount;
			_armParent->PlayAnimation("waving_right");
			_moveArrowX = 1;
		}

		if (global._trg & PAD_INPUT_1)
		{
			_selectButtons[_stageTypeNum]->GetMonitor()->SetDrawOrder(500000);
			for (auto&& button : _selectButtons[_stageTypeNum]->GetButtons())
			{
				button->SetDrawOrder(500001);
			}
			_selectButtons[_stageTypeNum]->GetMonitor()->PlayAnimation("Monitor");
			_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->PlayAnimation("Select");
			_isSelect = true;
		}
		if (global._trg & PAD_INPUT_9)
		{
			NEW MenuScreen(GetOwner());
		}
	}

	if (_frameCount > 0.f)
	{
		_frameCount -= 1.f;
		
		_pos = Vector2::Lerp(_from, _to, 1.f - _frameCount / _takeFrame);
		auto beforePos = Vector2::Lerp(_from, _to, 1.f - (_frameCount + 1.f) / _takeFrame);

		for (auto&& buttons : _selectButtons)
		{
			buttons->SetLocation(buttons->GetLocation() - (_pos - beforePos));
		}
		if (_frameCount < 1.f)
		{
			_pos = _from;
			_containerParent->ResetAnimation("sway");
			_terminal->ResetAnimation("sway");
			_armParent->PlayAnimation("waving_back");
			_moveArrowX = 0;
		}
	}
}

void StageSelectScreen::Draw()
{
	UIScreen::Draw();
}