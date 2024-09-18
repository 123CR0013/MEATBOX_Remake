#include"SettingScreen.h"
#include"SoundAdjustScreen.h"
#include"MoveUI.h"

constexpr unsigned int BGM = 0;
constexpr unsigned int SE = 1;
constexpr unsigned int BACK = 2;

constexpr float TAKE_FRAME = 20.f;

SettingScreen::SettingScreen(ModeUI* owner)
	:UIScreen(owner)
	, _buttonNum(0)
	,_BGMLevel(0)
	,_SELevel(0)
	, _isFinish(false)
{
	std::string fileName[] = {"ui_volume_bgm_01","ui_volume_se_01","ui_volume_back_01"};

	float poses[] = { -360.f,-180.f,180.f };

	_backGround = NEW Graph(this, 0);
	_backGround->Load("res/UI/Select/SoundAdjust/ui_volume_base_01.png");
	_backGround->SetLocation(180.f + _backGround->GetWidth() / 2.f, 60.f + _backGround->GetHeight() / 2.f);

	for (size_t i = 0; i < _buttons.size(); ++i)
	{
		_buttons[i] = NEW Graph(this,0);
		_buttons[i]->Load("res/UI/Select/SoundAdjust/" + fileName[i] + ".png");
		_buttons[i]->SetLocation(300.f - _backGround->GetLocation().x + _buttons[i]->GetWidth() / 2.f, poses[i] + _buttons[i]->GetHeight() / 2.f);
		_buttons[i]->RegistParent(_backGround);
	}

	for (size_t i = 0; i < _BGMLevelUIs.size(); ++i)
	{
		_BGMLevelUIs[i] = NEW Graph(this, 0);
		_BGMLevelUIs[i]->Load("res/UI/Select/SoundAdjust/ui_volume_control_01.png");
		_BGMLevelUIs[i]->SetWidth(50.f + 12.f * i);
		_BGMLevelUIs[i]->SetHeight(50.f + 12.f * i);
		_BGMLevelUIs[i]->SetLocation(900.f - _backGround->GetLocation().x + _BGMLevelUIs[i]->GetWidth() / 2.f + (132.f + 12.f) * (float)i, 228.f + _BGMLevelUIs[i]->GetHeight() / 2.f - 12.f * (float)i - _backGround->GetLocation().y);
		_BGMLevelUIs[i]->RegistParent(_backGround);
	}

	for (size_t i = 0; i < _SELevelUIs.size(); ++i)
	{
		_SELevelUIs[i] = NEW Graph(this, 0);
		_SELevelUIs[i]->Load("res/UI/Select/SoundAdjust/ui_volume_control_01.png");
		_SELevelUIs[i]->SetWidth(50.f + 12.f * i);
		_SELevelUIs[i]->SetHeight(50.f + 12.f * i);
		_SELevelUIs[i]->SetLocation(900.f - _backGround->GetLocation().x + _BGMLevelUIs[i]->GetWidth() / 2.f + (132.f + 12.f) * (float)i,  408.f + _SELevelUIs[i]->GetHeight() / 2.f - 12.f * (float)i - _backGround->GetLocation().y);
		_SELevelUIs[i]->RegistParent(_backGround);
	}

	CreateScaleAnim("Scale", 1.f, 1.f, 5);
	CreateLocationYAnim("Scale", -_backGround->GetHeight() / 4.f, 5,Easing::OUT_QUART);

	int screenH = ApplicationBase::GetInstance()->DispSizeH();
	_backGround->SetLocation(180.f + _backGround->GetWidth() / 2.f, screenH);
	_backGround->SetScale(0.f, 0.f);
	_backGround->PlayAnimation("Scale");

	CreateRotateAnim("Lean", PI / 12.f, 5);

	_buttons.front()->PlayAnimation("Lean");

	_select = NEW MoveUI(this);
	_select->Load("res/UI/Select/ui_selecticon_01.png");
	_select->SetLocation(_buttons.front()->GetLocation());
	_select->SetTakeFrame(TAKE_FRAME);
	_select->RegistParent(_backGround);
}

SettingScreen::~SettingScreen()
{
}

void SettingScreen::Update()
{
	UIScreen::Update();

	if (_isFinish)
	{
		if (_backGround->IsFinishAnimation())
		{
			DeleteUIScreen(this);
		}
		return;
	}

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
			_select->SetFrom(_select->GetLocation());
			_select->SetFrameCount(TAKE_FRAME);
			_backGround->ReverseAnimation();
			_isFinish = true;
			break;
		}

		default:
			break;
		}
	}

	if (global._trg & PAD_INPUT_3)
	{
		_select->SetFrom(_select->GetLocation());
		_select->SetFrameCount(TAKE_FRAME);
		_backGround->ReverseAnimation();
		_isFinish = true;
	}
}