#include"StageSelectScreen.h"
#include"StageSelectButton.h"
#include"MenuScreen.h"
#include"ModeGame.h"

constexpr float TAKE_FRAME = 30.f;

StageSelectScreen::StageSelectScreen(ModeUI* owner)
	:UIScreen(owner)
	,_frameCount(0)
	,_takeFrame(TAKE_FRAME)
	,_fromFrame(0)
	,_isSelect(false)
	,_worldID(0)
	,_stageID(0)
	,_moveArrowX(0)
{
	// 非同期読み込み設定
	SetUseASyncLoadFlag(TRUE);

	for (size_t i = 0;i<_selectButtons.size();++i) 
	{
		_selectButtons[i] = NEW StageSelectButton(this,i);
	}

	_pos = _selectButtons.front()->GetLocation();
	_from = _pos;

	_containerParent = NEW Empty(this,500);

	_container = NEW Graph(this,1001);
	_container->Load("res/UI/Select/Stage/ui_stage_container_01.png");
	_container->RegistParent(_containerParent);
	_container->SetLocation(0.f, -_container->GetHeight() / 2.f);

	_containerParent->SetLocation(1670.f, 700.f + _container->GetHeight() / 2.f);

	_meatBox = NEW Graph(this,1000);
	_meatBox->Load("res/UI/Select/Stage/ui_stage_boss_01.png");
	_meatBox->SetLocation(0.f, -_container->GetHeight() / 2.f);
	_meatBox->RegistParent(_containerParent);

	CreateLocationYAnim("UpDown", 20.f, 60, Easing::OUT_QUART);
	CreateLocationYAnim("UpDown", -20.f, 60, Easing::OUT_QUART);

	_meatBox->PlayAnimation("UpDown", 0);

	_terminal = NEW Graph(this,1000);
	_terminal->Load("res/UI/Select/Stage/ui_stage_terminal_01.png");
	_terminal->SetLocation(966.f, 834);

	_light = NEW FlipBook(this, 1000);
	_light->LoadDiv("res/UI/Select/Stage/ui_stage_light_01_sheet.png", 15, 15, 1, 10620 / 15, 218);
	_light->SetLocation(966.f, 19.f);
	_light->SetFlipSpeed(10);

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

	{
		_fadeBox = NEW Box(this, UINT_MAX);
		_fadeBox->SetDrawType(UI::DrawType::kLeft);
		_fadeBox->SetSize(1920.f, 1280.f);
		_fadeBox->SetColor(0, 0, 0);

		CreateOpacityAnim("FadeIn", -1.f, 60);
		CreateOpacityAnim("FadeOut", 1.f, 60);

		_fadeBox->PlayAnimation("FadeIn");
	}

	//操作説明UI
	{
		_tutorialArrow = NEW Graph(this,300);
		_tutorialArrow->Load("res/UI/ui_tutorial_arrow_02.png");
		_tutorialArrow->SetLeftLocation(30.f, 997.f);

		_tutorialSelect = NEW Graph(this,300);
		_tutorialSelect->Load("res/UI/ui_tutorial_select_01.png");
		_tutorialSelect->SetLeftLocation(184.f, 997.f);
		
		_tutorialMenu = NEW Graph(this,300);
		_tutorialMenu->Load("res/UI/ui_tutorial_menu_01.png");
		_tutorialMenu->SetLeftLocation(1464.f, 997.f);
	}

	// 同期読み込み設定
	SetUseASyncLoadFlag(FALSE);
}

StageSelectScreen::~StageSelectScreen(){}

void StageSelectScreen::Update() {
	UIScreen::Update();

	//フェード用アニメーションをしていたら、それ以上の処理をしない
	if (!_fadeBox->IsFinishAnimation()) { return; }

	for (size_t i = 0; i < _selectButtons[_worldID]->GetButtons().size(); ++i)
	{
		if (_selectButtons[_worldID]->GetButtons()[i]->IsRightClickTrg())
		{
			if (!_selectButtons[_worldID]->GetStageDatas()[i])
			{
				_selectButtons[_worldID]->GetStageDatas()[i] = true;
				_selectButtons[_worldID]->GetButtons()[i]->Load("res/UI/Select/Stage/ui_stage_folder_01.png");
			}
			else 
			{
				_selectButtons[_worldID]->GetStageDatas()[i] = false;
				_selectButtons[_worldID]->GetButtons()[i]->Load("res/UI/Select/Stage/ui_stage_folder_02.png");
			}
		}
	}

	if (_isSelect)
	{
		if (global._trg & PAD_INPUT_2)
		{
			std::function<void()>exe = [=]()mutable 
				{
					_selectButtons[_worldID]->GetMonitor()->SetDrawOrder(500);
					for (auto&& button : _selectButtons[_worldID]->GetButtons())
					{
						button->SetDrawOrder(501);
					}
				};
			ModeTimeTable::Add(exe, 20);

			_selectButtons[_worldID]->GetMonitor()->ReverseAnimation();
			_selectButtons[_worldID]->GetButtons()[_stageID]->ReverseAnimation();
			_stageID = 0;
			_isSelect = false;

			global._soundServer->Play("SE_05");
		}

		if (global._trg & PAD_INPUT_LEFT)
		{
			if (0 < _stageID) {
				if (_selectButtons[_worldID]->GetStageDatas()[_stageID - 1])
				{
					_selectButtons[_worldID]->GetButtons()[_stageID]->ReverseAnimation();
					_stageID--;
					_selectButtons[_worldID]->GetButtons()[_stageID]->PlayAnimation("Select");
				}
				global._soundServer->Play("SE_01");
			}
		}

		if (global._trg & PAD_INPUT_RIGHT )
		{

			if (_stageID < _selectButtons[_worldID]->GetButtons().size() - 1) {
				if (_selectButtons[_worldID]->GetStageDatas()[_stageID + 1])
				{
					_selectButtons[_worldID]->GetButtons()[_stageID]->ReverseAnimation();
					_stageID++;
					_selectButtons[_worldID]->GetButtons()[_stageID]->PlayAnimation("Select");
				}
				global._soundServer->Play("SE_01");
			}
		}

		if (global._trg & PAD_INPUT_1)
		{
			ModeServer::GetInstance()->Add(NEW ModeGame(_worldID + 1,_stageID + 1), 1, "ModeGame");
			ModeServer::GetInstance()->Del(GetOwner());
			global._soundServer->Play("SE_04");
		}
	}
	else
	{
		

		if (global._trg & PAD_INPUT_LEFT && _worldID > 0)
		{
			_worldID = _worldID - 1;
			_to = _selectButtons[_worldID]->GetLocation();
			_frameCount = _moveArrowX == 1 ? TAKE_FRAME / 2.f : TAKE_FRAME;
			_takeFrame = _frameCount;
			_armParent->PlayAnimation("waving_left");
			_moveArrowX = -1;
			global._soundServer->Play("SE_01");
		}

		if (global._trg & PAD_INPUT_RIGHT && _worldID < _selectButtons.size() - 1)
		{
			_worldID = _worldID + 1;
			_to = _selectButtons[_worldID]->GetLocation();
			_frameCount = _moveArrowX == -1 ? TAKE_FRAME / 2.f: TAKE_FRAME;
			_takeFrame = _frameCount;
			_armParent->PlayAnimation("waving_right");
			_moveArrowX = 1;
			global._soundServer->Play("SE_01");
		}

		if (global._trg & PAD_INPUT_1)
		{
			//一つでも選べる状態になっていれば
			bool canSelect = false;
			for (size_t i = 0; i < _selectButtons[_worldID]->GetButtons().size(); ++i)
			{
				if (_selectButtons[_worldID]->GetStageDatas()[i])
				{
					canSelect = true;
					_stageID = i;
					break;
				}
			}

			if (canSelect) 
			{
				_selectButtons[_worldID]->GetMonitor()->SetDrawOrder(500000);
				for (auto&& button : _selectButtons[_worldID]->GetButtons())
				{
					button->SetDrawOrder(500001);
				}
				_selectButtons[_worldID]->GetMonitor()->PlayAnimation("Monitor");
				_selectButtons[_worldID]->GetButtons()[_stageID]->PlayAnimation("Select");
				_isSelect = true;

				global._soundServer->Play("SE_04");
			}
		}
		if (global._trg & PAD_INPUT_9)
		{
			NEW MenuScreen(GetOwner());

			auto exe = [=]()mutable
			{
				_tutorialArrow->SetAlpha(0.f);
				_tutorialSelect->SetAlpha(0.f);
				_tutorialMenu->SetAlpha(0.f);
			};

			ModeTimeTable::Add(exe, 1);

			global._soundServer->Play("SE_04");
		}
		else
		{
			_tutorialArrow->SetAlpha(1.f);
			_tutorialMenu->SetAlpha(1.f);
			_tutorialSelect->SetAlpha(1.f);
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