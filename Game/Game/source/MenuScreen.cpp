#include"MenuScreen.h"
#include"SettingScreen.h"
#include"MoveUI.h"

constexpr unsigned int CONTINUE = 0;
constexpr unsigned int SETTING = 1;
constexpr unsigned int GAME_END = 2;

MenuScreen::MenuScreen(ModeUI* owner)
	:UIScreen(owner)
	,_buttonNum(0)
	,_isFinish(false)
{
	//UI���̓ǂݍ���
	CSVFile ifs("data/UI/MenuScreenParam.csv", CSVFile::Type::LINE);

	//�w�i
	_backGround = NEW Graph(this,0);
	_backGround->Load("res/UI/Select/Menu/ui_menu_base_01.png");
	_backGround->SetLeftLocation(ifs["BackGroundPosX"][0].GetFloat(), ifs["BackGroundPosY"][0].GetFloat());
	_backGround->SetScale(0.f, 0.f);

	//�w�i������A�j���[�V�����̈ړ���
	Vector2 to(60.f + _backGround->GetWidth() / 2.f, 60.f + _backGround->GetHeight() / 2.f);
	Vector2 diff = to - _backGround->GetLocation();

	CreateScaleAnim("Scale", 1.f,1.f, 20);
	CreateLocationAnim("Scale", diff.x,diff.y, 20, Easing::EleNumMap[ifs["Easing"][0].GetStr()]);

	_backGround->PlayAnimation("Scale");

	//�t�@�C����
	std::string names[] = { "ui_menu_resume_01","ui_menu_volume_01","ui_menu_quit_01" };

	//�z�u�ʒu
	float poses[] = {180.f,360.f,720.f};

	//�w�i�ʒu�̋t�s��
	Matrix3 inMatBackGround = Matrix3::CreateTranslation(60.f + _backGround->GetWidth() / 2.f, 60.f + _backGround->GetHeight() / 2.f).Invert();

	//���j���[�{�^���̐���
	for (size_t i =0;i< _buttons.size();++i)
	{
		_buttons[i] = NEW Graph(this,1);
		_buttons[i]->Load("res/UI/Select/Menu/" + names[i] + ".png");
		_buttons[i]->SetLeftLocation(Vector2(ifs["ButtonPosX"][i].GetFloat(), ifs["ButtonPosY"][i].GetFloat()) * inMatBackGround);
		_buttons[i]->RegistParent(_backGround);
	}

	//20�x�E�ɌX���A�j���[�V����
	CreateRotateAnim("Lean", PI / 18.f, 5);

	_buttons.front()->PlayAnimation("Lean");

	//�J�[�\��(�I�����Ă��郁�j���[�{�^�����w��)
	_carsol = NEW MoveUI(this);
	_carsol->Load("res/UI/Select/ui_selecticon_01.png");
	_carsol->SetLocation(_buttons.front()->GetLocation());
	_carsol->SetTakeFrame(ifs["Frame"][0].GetFloat());
	_carsol->RegistParent(_backGround);

	//�������UI
	{
		_tutorialArrow = NEW Graph(this);
		_tutorialArrow->Load("res/UI/ui_tutorial_arrow_01.png");
		_tutorialArrow->SetLeftLocation(30.f,997.f);

		_tutorialSelect = NEW Graph(this);
		_tutorialSelect->Load("res/UI/ui_tutorial_select_01.png");
		_tutorialSelect->SetLeftLocation(184.f, 997.f);

		_tutorialReturn = NEW Graph(this);
		_tutorialReturn->Load("res/UI/ui_tutorial_return_01.png");
		_tutorialReturn->SetLeftLocation(993.f, 997.f);
	}
}

MenuScreen::~MenuScreen(){}

void MenuScreen::Update()
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
		if (_buttonNum > 0)
		{
			_buttons[_buttonNum]->ReverseAnimation();
			_buttonNum--;
			_buttons[_buttonNum]->PlayAnimation("Lean");

			_carsol->SetFrom(_carsol->GetLocation());
			_carsol->SetTo(_buttons[_buttonNum]->GetLocation());
			_carsol->SetFrameCount(0.f);

			global._soundServer->Play("SE_01");
		}
	}
	if (global._trg & PAD_INPUT_DOWN)
	{
		if (_buttonNum < _buttons.size() - 1)
		{
			_buttons[_buttonNum]->ReverseAnimation();
			_buttonNum++;
			_buttons[_buttonNum]->PlayAnimation("Lean");

			_carsol->SetFrom(_carsol->GetLocation());
			_carsol->SetTo(_buttons[_buttonNum]->GetLocation());
			_carsol->SetFrameCount(0.f);

			global._soundServer->Play("SE_01");
		}
	}

	if (global._trg & PAD_INPUT_1) {
		switch (_buttonNum)
		{
		case(CONTINUE):
		{
			_backGround->ReverseAnimation();
			_isFinish = true;
			break;
		};

		case(SETTING):
		{
			NEW SettingScreen(_carsol,GetOwner());
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
		global._soundServer->Play("SE_04");
		auto exe = [=]()mutable
		{
			_tutorialArrow->SetAlpha(0.f);
			_tutorialSelect->SetAlpha(0.f);
			_tutorialReturn->SetAlpha(0.f);
		};

		ModeTimeTable::Add(exe, 1);
	}
	else
	{
		_tutorialArrow->SetAlpha(1.f);
		_tutorialSelect->SetAlpha(1.f);
		_tutorialReturn->SetAlpha(1.f);
	}

	if (global._trg & PAD_INPUT_2)
	{
		_backGround->ReverseAnimation();
		_isFinish = true;
		global._soundServer->Play("SE_05");
	}
}

void MenuScreen::Draw()
{
	UIScreen::Draw();
}