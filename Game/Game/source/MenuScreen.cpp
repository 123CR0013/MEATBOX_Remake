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

	std::string names[] = {"ui_menu_resume_01","ui_menu_volume_01","ui_menu_quit_01"};

	auto backGround = NEW Graph(this,0);
	backGround->Load("res/UI/Select/Menu/ui_menu_base_01.png");
	backGround->SetLocation(60.f + backGround->GetWidth() / 2.f, 60.f + backGround->GetHeight() / 2.f);

	float poses[] = {180.f,360.f,720.f};

	for (size_t i =0;i< _buttons.size();++i)
	{
		_buttons[i] = NEW Graph(this,1);
		_buttons[i]->Load("res/UI/Select/Menu/" + names[i] + ".png");
		_buttons[i]->SetLocation(240.f + _buttons[i]->GetWidth() / 2.f,  poses[i] + _buttons[i]->GetHeight() / 2.f);
	}

	CreateRotateAnim("Lean", PI / 18.f, 5);

	_buttons.front()->PlayAnimation("Lean");

	_select = NEW MoveUI(this);
	_select->Load("res/UI/Select/ui_selecticon_01.png");
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