#include"ResultScreen.h"
#include"ModeGame.h"
#include"ModeSelect.h"
#include"Number.h"

constexpr USHORT NEXT = 0;
constexpr USHORT RETRY = 1;
constexpr USHORT BACK = 2;

ResultScreen::ResultScreen(ModeUI* owner)
	:UIScreen(owner)
	,_stepCount(0)
	,_killCount(0)
	,_frameCount(0)
	,_step(Step::kMainFrameAnimation)
{

	_mainFrame = NEW Graph(this);
	_mainFrame->Load("res/UI/Result/ui_result_base_01.png");
	_mainFrame->SetLocation(975.f, 1040.f);
	_mainFrame->SetAlpha(0.f);

	CreateLocationYAnim("Up", 510.f - 1040.f, 20, Easing::OUT_QUART);
	CreateOpacityAnim("Up", 1.f, 20, Easing::OUT_QUART);

	_mainFrame->PlayAnimation("Up");

	const Matrix3 inMatMainFrame = Matrix3::CreateTranslation(975.f, 510.f).Invert();
	{
		//_mainFrameのアニメーション後の逆座標行列
		//プランナーから指定された座標がアニメーション後のものだったため

		_itemStepCounter = NEW Graph(this);
		_itemStepCounter->Load("res/UI/Result/ui_result_stepcount_01.png");
		_itemStepCounter->SetLocation(Vector2(875.f, 353.f) * inMatMainFrame);
		_itemStepCounter->SetAlpha(0.f);
		_itemStepCounter->RegistParent(_mainFrame);

		_stepCounter = NEW Number(this, "res/UI/Result/number/ui_result_count_0");
		_stepCounter->SetLocation(Vector2(1106.f, 353.f) * inMatMainFrame);
		_stepCounter->SetFormat(Number::Format::kLeft);
		_stepCounter->SetAlpha(0.f);
		_stepCounter->RegistParent(_mainFrame);

		_itemKillCounter = NEW Graph(this);
		_itemKillCounter->Load("res/UI/Result/ui_result_killcount_01.png");
		_itemKillCounter->SetLocation(Vector2(875.f,548.f) * inMatMainFrame);
		_itemKillCounter->SetAlpha(0.f);
		_itemKillCounter->RegistParent(_mainFrame);

		_killCounter = NEW Number(this, "res/UI/Result/number/ui_result_count_0");
		_killCounter->SetLocation(Vector2(1106.f, 548.f) * inMatMainFrame);
		_killCounter->SetFormat(Number::Format::kLeft);
		_killCounter->SetAlpha(0.f);
		_killCounter->RegistParent(_mainFrame);

		CreateLocationXAnim("SlideItem", 100.f, 20);
		CreateOpacityAnim("SlideItem", 1.f, 20);
	}

	{
		_gaugeFrame = NEW Graph(this,1000);
		_gaugeFrame->SetDrawType(UI::DrawType::kLeft);
		_gaugeFrame->Load("res/UI/Result/ui_result_resultgauge_01.png");

		int screenHandle = MakeScreen(1920, 1080, TRUE);
		int blendHandle = MakeScreen(1920, 1080, TRUE);

		//切り抜くための図形を描画
		SetDrawScreen(screenHandle);
		ClearDrawScreen();
		
		DrawBox(0, 0, 1920, 1080, GetColor(0, 255, 0), TRUE);

		//切り抜き
		GraphBlendBlt(screenHandle, _gaugeFrame->GetHandle(), blendHandle, 255,
			DX_GRAPH_BLEND_RGBA_SELECT_MIX,
			DX_RGBA_SELECT_SRC_R,
			DX_RGBA_SELECT_SRC_G,
			DX_RGBA_SELECT_SRC_B,
			DX_RGBA_SELECT_BLEND_A
		);

		// 描画先を裏にする
		SetDrawScreen(DX_SCREEN_BACK);

		//削除
		DeleteGraph(screenHandle);

		//輝度の低い部分を透過
		GraphFilter(_gaugeFrame->GetHandle(), DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 5, TRUE, 0, 0);

		_gaugeBar = NEW Graph(this);
		_gaugeBar->SetHandle(blendHandle);
		_gaugeBar->SetDrawType(UI::DrawType::kLeft);
		_gaugeBar->SetSize(_gaugeFrame->GetSize());

		_gaugeClip = NEW Clip(this);
		_gaugeClip->SetDrawType(UI::DrawType::kLeft);
		_gaugeClip->SetSize(_gaugeFrame->GetSize());
		_gaugeClip->AddChild(_gaugeBar);
		_gaugeClip->SetAlpha(0.f);

		_gaugeClip->SetWidth(0.f);
		_gaugeFrame->RegistParent(_mainFrame);
		_gaugeClip->RegistParent(_mainFrame);
		_gaugeFrame->SetAlpha(0.f);

		Vector2 pos(-100.f, 270.f);
		pos = pos - _gaugeFrame->GetSize() / 2.f;
		_gaugeFrame->SetLocation(pos);
		_gaugeClip->SetLocation(pos);
	}

	CreateLocationXAnim("MainFrameSlide", -410.f, 20, Easing::OUT_QUART);

	{
		Vector2 subFramePos(1468.f, 510.f);
		_subFrame = NEW Graph(this);
		_subFrame->Load("res/UI/Result/ui_result_base_02.png");
		_subFrame->SetLocation(1468.f+ 410.f, 510.f);
		_subFrame->SetAlpha(0.f);

		CreateLocationXAnim("SubFrameSlide", -410.f, 20, Easing::OUT_QUART);
		CreateOpacityAnim("SubFrameSlide", 1.f, 20, Easing::OUT_QUART);

		Matrix3 inMatSubFrame = Matrix3::CreateTranslation(subFramePos).Invert();

		Vector2 pos[] =
		{
			{1468.f,290.f},
			{1468.f,530.f},
			{1468.f,770.f}
		};

		std::string fileName[] =
		{
			{"res/UI/Result/ui_result_next_01.png"},
			{"res/UI/Result/ui_result_retry_01.png"},
			{"res/UI/Result/ui_result_back_01.png"}
		};

		for (size_t i = 0; i < _buttons.size(); ++i)
		{
			_buttons[i] = NEW Graph(this);
			_buttons[i]->Load(fileName[i]);
			_buttons[i]->SetLocation(pos[i] * inMatSubFrame);
			_buttons[i]->SetAlpha(0.f);
			_buttons[i]->RegistParent(_subFrame);
		}

		_selectNum = 0;

		CreateOpacityAnim("ButtonAlpha", 1.f, 20, Easing::OUT_QUART);

		CreateScaleAnim("ButtonScale", 0.1f, 0.1f, 5);
	}
}

ResultScreen::~ResultScreen()
{
	DeleteGraph(_gaugeBar->GetHandle());
}

void ResultScreen::Update()
{

	UIScreen::Update();

	switch (_step)
	{
	case Step::kNone:	//何もしない
		break;

	case Step::kMainFrameAnimation:
		if (_mainFrame->IsFinishAnimation())
		{
			_itemStepCounter->PlayAnimation("SlideItem");
			_stepCounter->PlayAnimation("SlideItem");

			std::function<void()>exe = [=]()mutable {_step = Step::kStepCount; };

			ModeTimeTable::Add(exe, 20);
		}
		break;

	case Step::kStepCount:
		
		_stepCounter->SetNumber(static_cast<UINT>(Easing::Linear(static_cast<float>(++_frameCount), 0.f, 102.f, 60.f)));

		if (_frameCount == 60)
		{
			//20フレームは何もしない
			_step = Step::kNone;	

			_itemKillCounter->PlayAnimation("SlideItem");
			_killCounter->PlayAnimation("SlideItem");

			std::function<void()>exe = [=]()mutable {_step = Step::kKillCount; };

			ModeTimeTable::Add(exe, 20);

			_frameCount = 0;
		}

		break;

	case Step::kKillCount:

		_killCounter->SetNumber(static_cast<UINT>(Easing::Linear(static_cast<float>(++_frameCount), 0.f, 102.f, 60.f)));

		if (_frameCount == 60)
		{
			//20フレームは何もしない
			_step = Step::kNone;

			_gaugeClip->PlayAnimation("SlideItem");
			_gaugeFrame->PlayAnimation("SlideItem");

			std::function<void()>exe = [=]()mutable {_step = Step::kGauge; };

			ModeTimeTable::Add(exe, 20);

			_frameCount = 0;
		}
		break;
	case Step::kGauge:

		if(_gaugeClip->IsFinishAnimation())
			_gaugeClip->SetWidth(_gaugeBar->GetWidth() * Easing::Linear(static_cast<float>(++_frameCount), 0.f, 1.f, 60.f));


		if (_frameCount == 60)
		{
			_step = Step::kNone;

			_mainFrame->PlayAnimation("MainFrameSlide");
			_subFrame->PlayAnimation("SubFrameSlide");

			for (size_t i = 0; i < _buttons.size(); ++i)
			{
				_buttons[i]->PlayAnimation("ButtonAlpha");
			}

			std::function<void()>exe = [=]()mutable {_step = Step::kChoice; _buttons.front()->PlayAnimation("ButtonScale"); };

			ModeTimeTable::Add(exe, 20);

			_frameCount = 0;
		}
		break;
	case Step::kChoice:

		if (global._trg & PAD_INPUT_UP && _selectNum > 0)
		{
			_buttons[_selectNum]->ReverseAnimation();
			_selectNum = _selectNum - 1;
			_buttons[_selectNum]->PlayAnimation("ButtonScale");
		}

		if (global._trg & PAD_INPUT_DOWN && _selectNum < _buttons.size() - 1)
		{
			_buttons[_selectNum]->ReverseAnimation();
			_selectNum = _selectNum + 1;
			_buttons[_selectNum]->PlayAnimation("ButtonScale");
		}

		if (global._trg & PAD_INPUT_1)
		{
			switch (_selectNum)
			{
				case NEXT:
				{
					break;
				}
				case RETRY:
				{
					break;
				}
				case BACK:
				{
					ModeServer::GetInstance()->Del(GetOwner());
					ModeServer::GetInstance()->Add(NEW ModeSelect(),1,"ModeSelect");
					break;
				}
			}
		}

		break;
	}

	
}

void ResultScreen::Draw()
{

	UIScreen::Draw();
}