#include"ResultScreen.h"
#include"ModeGame.h"
#include"ModeSelect.h"
#include"Number.h"
#include"ClearScreen.h"

constexpr USHORT NEXT = 0;
constexpr USHORT RETRY = 1;
constexpr USHORT BACK = 2;

ResultScreen::ResultScreen(ModeGame* owner)
	:UIScreen(owner)
	,_stepCount(0)
	,_killCount(0)
	,_frameCount(0)
	,_step(Step::kMainFrameAnimation)
	, _resultData(owner->GetResultData())
	, _worldID(owner->GetWorldID())
	, _stageID(owner->GetStageID())
	,_worldIDNext(-1)
	,_stageIDNext(-1)
	,_game(owner)
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
		int gaugeFrame = LoadGraph("res/UI/Result/ui_result_resultgauge_01.png");

		//切り抜き
		GraphBlendBlt(screenHandle, gaugeFrame, blendHandle, 255,
			DX_GRAPH_BLEND_RGBA_SELECT_MIX,
			DX_RGBA_SELECT_SRC_R,
			DX_RGBA_SELECT_SRC_G,
			DX_RGBA_SELECT_SRC_B,
			DX_RGBA_SELECT_BLEND_A
		);

		DeleteGraph(gaugeFrame);

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

		Vector2 pos(795.f, 779.f);
		pos = pos - _gaugeFrame->GetSize() / 2.f;
		pos = pos * inMatMainFrame;
		_gaugeFrame->SetLocation(pos);
		_gaugeClip->SetLocation(pos);
		auto p = _gaugeFrame->GetWorldLocation();
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
			_buttons[i] = NEW Button(this);
			_buttons[i]->SetLocation(pos[i] * inMatSubFrame);
			_buttons[i]->SetAlpha(0.f);
			_buttons[i]->RegistParent(_subFrame);

			Graph* graph = NEW Graph(this);
			graph->Load(fileName[i]);
			graph->SetAlpha(0.f);
			graph->RegistParent(_buttons[i]);
		}

		_selectNum = 0;

		CreateOpacityAnim("ButtonAlpha", 1.f, 20, Easing::OUT_QUART);

		CreateScaleAnim("ButtonScale", 0.1f, 0.1f, 5);
	}

	{
		_rank = NEW Graph(this,1000);
		_rank->Load("res/UI/Result/ui_result_rank_01.png");
		_rank->SetAlpha(0.f);
		_rank->SetScale(2.f, 2.f);
		_rank->SetLocation(Vector2(1287.f, 812.f)* inMatMainFrame);
		_rank->RegistParent(_mainFrame);

		CreateScaleAnim("Rank", -1.f, -1.f, 60);
		CreateOpacityAnim("Rank", 1.f, 60);
	}

	//ボタン処理
	{
		ZFile ifs("data/nextstagedata_" + std::to_string(_worldID) + "_" + std::to_string(_stageID) + ".amg");
		if (ifs.Success())
		{
			CSVFile::Cell _cellData;
			CSVFile::Parse(ifs.DataStr(), CSVFile::Type::LINE, _cellData);


			if (_cellData["Command"][0].GetStr() == "Next")
			{
				_worldIDNext = _cellData["NextStage"][0].GetInt();
				_stageIDNext = _cellData["NextStage"][1].GetInt();

				_buttons[NEXT]->SetExe(
					[=]()mutable {
						_game->ChangeStage(_worldIDNext, _stageIDNext);
					}
				);
			}
			else if(_cellData["Command"][0].GetStr() == "End")
			{
				_buttons[NEXT]->SetExe(
					[=]()mutable {
						NEW ClearScreen(GetOwner());
					}
				);
			}
		}

		_buttons[RETRY]->SetExe(
			[=]()mutable {
				_game->ChangeStage(_worldID, _stageID);
			}
		);

		_buttons[BACK]->SetExe(
			[=]()mutable {
				ModeServer::GetInstance()->Del(GetOwner());
				ModeServer::GetInstance()->Add(NEW ModeSelect(), 1, "ModeSelect");
			}
		);
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
		
		_stepCounter->SetNumber(_stepCounter->GetNumber() + 1);
		if(_frameCount++ % 5 == 0)global._soundServer->Play("SE_02");

		if (_stepCounter->GetNumber() == 102)
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

		_killCounter->SetNumber(_killCounter->GetNumber() + 1);
		if (_frameCount++ % 5 == 0)global._soundServer->Play("SE_02");

		if (_resultData.killCntMax == _killCounter->GetNumber())
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
			if(_frameCount % 5 == 0)global._soundServer->Play("SE_06");

		if (_frameCount == 60)
		{
			_step = Step::kNone;

			_rank->PlayAnimation("Rank");

			std::function<void()>exe = [=]()mutable {_step = Step::kRank; };

			ModeTimeTable::Add(exe, 20);

			_frameCount = 0;
		}

		break;
	case Step::kRank:
	{
		if (_frameCount++ == 60)
		{
			_step = Step::kNone;

			_mainFrame->PlayAnimation("MainFrameSlide");
			_subFrame->PlayAnimation("SubFrameSlide");

			for (size_t i = 0; i < _buttons.size(); ++i)
			{
				if (_buttons[i]->GetChildren().size() > 0)_buttons[i]->GetChildren().front()->PlayAnimation("ButtonAlpha");
			}

			std::function<void()>exe = [=]()mutable {_step = Step::kChoice; if (_buttons.front()->GetChildren().size() > 0)_buttons.front()->GetChildren().front()->PlayAnimation("ButtonScale"); };

			ModeTimeTable::Add(exe, 20);

			_frameCount = 0;
		}
		break;
	}
	case Step::kChoice:

		if (global._trg & PAD_INPUT_UP && _selectNum > 0)
		{
			if (_buttons[_selectNum]->GetChildren().size() > 0)_buttons[_selectNum]->GetChildren().front()->ReverseAnimation();
			_selectNum = _selectNum - 1;
			if (_buttons[_selectNum]->GetChildren().size() > 0)_buttons[_selectNum]->GetChildren().front()->PlayAnimation("ButtonScale");

			global._soundServer->Play("SE_01");
		}

		if (global._trg & PAD_INPUT_DOWN && _selectNum < _buttons.size() - 1)
		{
			if (_buttons[_selectNum]->GetChildren().size() > 0)_buttons[_selectNum]->GetChildren().front()->ReverseAnimation();
			_selectNum = _selectNum + 1;
			if (_buttons[_selectNum]->GetChildren().size() > 0)_buttons[_selectNum]->GetChildren().front()->PlayAnimation("ButtonScale");
			global._soundServer->Play("SE_01");
		}

		if (global._trg & PAD_INPUT_1)
		{
			_buttons[_selectNum]->Push();

			global._soundServer->Play("SE_03");
		}

		break;
	}

	
}

void ResultScreen::Draw()
{
	UIScreen::Draw();

}