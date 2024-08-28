#include"MenuScreen.h"
#include"SettingScreen.h"
#include"MoveUI.h"

constexpr unsigned int CONTINUE = 0;
constexpr unsigned int SETTING = 1;
constexpr unsigned int GAME_END = 2;

constexpr float TAKE_FRAME = 20;

MenuScreen::MenuScreen(ModeUI* owner)
	:UIScreen(owner)
	,_buttonNum(0)
{

	char texts[3][255] = { "ë±ÇØÇÈ","ê›íË","Ç‚ÇﬂÇÈ" };

	for (size_t i =0;i< _buttons.size();++i)
	{
		_buttons[i] = NEW Box(this,0);
		_buttons[i]->SetColor(255, 255, 255);
		_buttons[i]->SetWidth(200.f);
		_buttons[i]->SetHeight(50.f);
		_buttons[i]->SetLocation(150.f, 200.f + 100.f* (float)i);


		Text* text = NEW Text(this);
		text->SetColor(255, 0, 0);
		text->SetTextSize(30.f);
		text->SetWidth(200.f);
		text->SetHeight(15.f);
		text->SetText(texts[i]);
		text->RegistParent(_buttons[i]);
	}

	CreateRotateAnim("Lean", PI / 12.f, 5);

	_buttons.front()->PlayAnimation("Lean");

	_select = NEW MoveUI(this);
	_select->SetColor(0, 255, 0);
	_select->SetWidth(50.f);
	_select->SetLocation(_buttons.front()->GetLocation());
	_select->SetTakeFrame(TAKE_FRAME);
}

MenuScreen::~MenuScreen(){}

void MenuScreen::Update()
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
		case(CONTINUE):
		{
			DeleteUIScreen(this);
			break;
		};

		case(SETTING):
		{
			NEW SettingScreen(GetOwner(), _select, _buttons[_buttonNum]->GetLocation());
			break;
		};

		case(GAME_END):
		{
			global.SetGameEnd(true);
			break;
		}

		default:
			break;
		}
	}

	if (global._trg & PAD_INPUT_3)
	{
		DeleteUIScreen(this);
	}
}