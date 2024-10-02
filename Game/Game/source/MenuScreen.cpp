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
	//UI情報の読み込み
	CSVFile ifs("data/UI/MenuScreenParam.csv", CSVFile::Type::LINE);

	//画面の縦幅
	int screenH = ApplicationBase::GetInstance()->DispSizeH();

	//背景
	_backGround = NEW Graph(this,0);
	_backGround->Load("res/UI/Select/Menu/ui_menu_base_01.png");
	_backGround->SetLeftLocation(ifs["BackGroundPosX"][0].GetFloat(), ifs["BackGroundPosY"][0].GetFloat());
	_backGround->SetScale(0.f, 0.f);

	//背景がするアニメーションの移動量
	Vector2 to(60.f + _backGround->GetWidth() / 2.f, 60.f + _backGround->GetHeight() / 2.f);
	Vector2 diff = to - _backGround->GetLocation();

	CreateScaleAnim("Scale", 1.f,1.f, 20);
	CreateLocationAnim("Scale", diff.x,diff.y, 20, Easing::EleNumMap[ifs["Easing"][0].GetStr()]);

	_backGround->PlayAnimation("Scale");

	//ファイル名
	std::string names[] = { "ui_menu_resume_01","ui_menu_volume_01","ui_menu_quit_01" };

	//配置位置
	float poses[] = {180.f,360.f,720.f};

	//背景位置の逆行列
	Matrix3 inMatBackGround = Matrix3::CreateTranslation(60.f + _backGround->GetWidth() / 2.f, 60.f + _backGround->GetHeight() / 2.f).Invert();

	//メニューボタンの生成
	for (size_t i =0;i< _buttons.size();++i)
	{
		_buttons[i] = NEW Graph(this,1);
		_buttons[i]->Load("res/UI/Select/Menu/" + names[i] + ".png");
		_buttons[i]->SetLeftLocation(Vector2(ifs["ButtonPosX"][i].GetFloat(), ifs["ButtonPosY"][i].GetFloat()) * inMatBackGround);
		_buttons[i]->RegistParent(_backGround);
	}

	//20度右に傾くアニメーション
	CreateRotateAnim("Lean", PI / 18.f, 5);

	_buttons.front()->PlayAnimation("Lean");

	//カーソル(選択しているメニューボタンを指す)
	_carsol = NEW MoveUI(this);
	_carsol->Load("res/UI/Select/ui_selecticon_01.png");
	_carsol->SetLocation(_buttons.front()->GetLocation());
	_carsol->SetTakeFrame(ifs["Frame"][0].GetFloat());
	_carsol->RegistParent(_backGround);
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
		_buttons[_buttonNum]->ReverseAnimation();
		_buttonNum = _buttonNum == 0 ? 0 : _buttonNum - 1;
		_buttons[_buttonNum]->PlayAnimation("Lean");

		_carsol->SetFrom(_carsol->GetLocation());
		_carsol->SetTo(_buttons[_buttonNum]->GetLocation());
		_carsol->SetFrameCount(0.f);
	}
	if (global._trg & PAD_INPUT_DOWN)
	{
		_buttons[_buttonNum]->ReverseAnimation();
		_buttonNum = _buttonNum == _buttons.size() - 1 ? _buttons.size() - 1 : _buttonNum + 1;
		_buttons[_buttonNum]->PlayAnimation("Lean");

		_carsol->SetFrom(_carsol->GetLocation());
		_carsol->SetTo(_buttons[_buttonNum]->GetLocation());
		_carsol->SetFrameCount(0.f);
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
	}

	if (global._trg & PAD_INPUT_3)
	{
		_backGround->ReverseAnimation();
		_isFinish = true;
	}
}