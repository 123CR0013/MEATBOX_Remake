#include"SettingScreen.h"
#include"SoundAdjustScreen.h"
#include"MoveUI.h"

constexpr unsigned int BGM = 0;
constexpr unsigned int SE = 1;
constexpr unsigned int BACK = 2;

constexpr float TAKE_FRAME = 20.f;

SettingScreen::SettingScreen(ModeUI* owner,MoveUI* select, const Vector2& backPos)
	:UIScreen(owner)
	,_select(select)
	, _buttonNum(0)
	,_BGMLevel(0)
	,_SELevel(0)
	,_backPos(backPos)
{
	std::string fileName[] = {"ui_volume_bgm_01","ui_volume_se_01","ui_volume_back_01"};

	float poses[] = { 180.f,360.f,720.f };

	auto backGround = NEW Graph(this, 0);
	backGround->Load("res/UI/Select/SoundAdjust/ui_volume_base_01.png");
	backGround->SetLocation(180.f + backGround->GetWidth() / 2.f, 60.f + backGround->GetHeight() / 2.f);

	for (size_t i = 0; i < _buttons.size(); ++i)
	{
		_buttons[i] = NEW Graph(this,0);
		_buttons[i]->Load("res/UI/Select/SoundAdjust/" + fileName[i] + ".png");
		_buttons[i]->SetLocation(300.f + _buttons[i]->GetWidth() / 2.f, poses[i] + _buttons[i]->GetHeight() / 2.f);
	}

	for (size_t i = 0; i < _BGMLevelUIs.size(); ++i)
	{
		_BGMLevelUIs[i] = NEW Graph(this, 0);
		_BGMLevelUIs[i]->Load("res/UI/Select/SoundAdjust/ui_volume_control_01.png");
		_BGMLevelUIs[i]->SetWidth(50.f + 12.f * i);
		_BGMLevelUIs[i]->SetHeight(50.f + 12.f * i);
		_BGMLevelUIs[i]->SetLocation(900.f + (132.f + 12.f) * (float)i, 228.f - 6.f * (float)i);
	}

	for (size_t i = 0; i < _SELevelUIs.size(); ++i)
	{
		_SELevelUIs[i] = NEW Graph(this, 0);
		_SELevelUIs[i]->Load("res/UI/Select/SoundAdjust/ui_volume_control_01.png");
		_SELevelUIs[i]->SetWidth(50.f + 12.f * i);
		_SELevelUIs[i]->SetHeight(50.f + 12.f * i);
		_SELevelUIs[i]->SetLocation(900.f + (132.f + 12.f) * (float)i, 448.f - 6.f * (float)i);
	}

	CreateRotateAnim("Lean", PI / 12.f, 5);

	_buttons.front()->PlayAnimation("Lean");

	_select->SetFrom(_select->GetLocation());
	_select->SetTo(_buttons[_buttonNum]->GetLocation());
	_select->SetFrameCount(TAKE_FRAME);

	RegistUI(_select);
}

SettingScreen::~SettingScreen()
{
	RemoveUI(_select);
}

void SettingScreen::Update()
{
	UIScreen::Update();

	if (global._trg & PAD_INPUT_UP)
	{
		_buttons[_buttonNum]->ReverseAnimation();
		_buttonNum = _buttonNum == 0 ? 0 : _buttonNum - 1;
		_buttons[_buttonNum]->PlayAnimation("Lean");

		_select->SetFrom(_select->GetLocation());
		_select->SetTo(_buttons[_buttonNum]->GetLocation());
		_select->SetFrameCount(TAKE_FRAME);
	}
	if (global._trg & PAD_INPUT_DOWN)
	{
		_buttons[_buttonNum]->ReverseAnimation();
		_buttonNum = _buttonNum == _buttons.size() - 1 ? _buttons.size() - 1 : _buttonNum + 1;
		_buttons[_buttonNum]->PlayAnimation("Lean");

		_select->SetFrom(_select->GetLocation());
		_select->SetTo(_buttons[_buttonNum]->GetLocation());
		_select->SetFrameCount(TAKE_FRAME);
	}

	if (global._trg & PAD_INPUT_1) {
		switch (_buttonNum)
		{
		case(BGM):
		{

			NEW SoundAdjustScreen(GetOwner(), _select, &_BGMLevelUIs, _buttons[_buttonNum]->GetLocation(),SoundItemBase::TYPE::BGM);
			break;
		};

		case(SE):
		{			
			NEW SoundAdjustScreen(GetOwner(), _select, &_SELevelUIs, _buttons[_buttonNum]->GetLocation(), SoundItemBase::TYPE::SE);
			break;
		};

		case(BACK):
		{
			DeleteUIScreen(this);
			_select->SetFrom(_select->GetLocation());
			_select->SetTo(_backPos);
			_select->SetFrameCount(TAKE_FRAME);
			break;
		}

		default:
			break;
		}
	}

	if (global._trg & PAD_INPUT_3)
	{
		DeleteUIScreen(this);
		_select->SetFrom(_select->GetLocation());
		_select->SetTo(_backPos);
		_select->SetFrameCount(TAKE_FRAME);
	}
}