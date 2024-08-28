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

	char texts[3][255] = { "BGM","SE","–ß‚é" };

	for (size_t i = 0; i < _buttons.size(); ++i)
	{
		_buttons[i] = NEW Box(this,0);
		_buttons[i]->SetColor(255, 125, 125);
		_buttons[i]->SetWidth(200.f);
		_buttons[i]->SetHeight(50.f);
		_buttons[i]->SetLocation(150.f, 200.f + 100.f * (float)i);


		Text* text = NEW Text(this);
		text->SetColor(255, 0, 0);
		text->SetTextSize(30.f);
		text->SetWidth(200.f);
		text->SetHeight(15.f);
		text->SetText(texts[i]);
		text->RegistParent(_buttons[i]);
	}

	for (size_t i = 0; i < _BGMLevelUIs.size(); ++i)
	{
		_BGMLevelUIs[i] = NEW Box(this, 0);
		_BGMLevelUIs[i]->SetColor(125, 125, 125);
		_BGMLevelUIs[i]->SetWidth(50.f);
		_BGMLevelUIs[i]->SetHeight(50.f);
		_BGMLevelUIs[i]->SetLocation(300.f + 75.f * (float)i, 200.f);
	}

	for (size_t i = 0; i < _SELevelUIs.size(); ++i)
	{
		_SELevelUIs[i] = NEW Box(this, 0);
		_SELevelUIs[i]->SetColor(125, 125, 125);
		_SELevelUIs[i]->SetWidth(50.f);
		_SELevelUIs[i]->SetHeight(50.f);
		_SELevelUIs[i]->SetLocation(300.f + 75.f * (float)i, 300.f);
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