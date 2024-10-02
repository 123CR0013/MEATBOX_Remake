#include"SettingScreen.h"
#include"SoundAdjustScreen.h"
#include"MoveUI.h"

constexpr unsigned int BGM = 0;
constexpr unsigned int SE = 1;
constexpr unsigned int BACK = 2;

constexpr float TAKE_FRAME = 5.f;

SettingScreen::SettingScreen(MoveUI* carsol,ModeUI* owner)
	:UIScreen(owner)
	, _buttonNum(0)
	,_BGMLevel(0)
	,_SELevel(0)
	,_carsol(carsol)
	, _inMatCarsolParent(Matrix3::CreateTranslation(carsol->GetParent()->GetWorldLocation()).Invert())
	, _backPos(carsol->GetLocation())
	, _isFinish(false)
{
	std::string fileName[] = {"ui_volume_bgm_01","ui_volume_se_01","ui_volume_back_01"};

	float poses[] = { 180.f,360.f,720.f };
	
	_backGround = NEW Graph(this, 0);
	_backGround->Load("res/UI/Select/SoundAdjust/ui_volume_base_01.png");
	_backGround->SetLeftLocation(180.f , 60.f);

	Matrix3 matInBackGround = Matrix3::CreateTranslation(_backGround->GetLocation()).Invert();

	for (size_t i = 0; i < _buttons.size(); ++i)
	{
		_buttons[i] = NEW Graph(this,0);
		_buttons[i]->Load("res/UI/Select/SoundAdjust/" + fileName[i] + ".png");
		_buttons[i]->SetLeftLocation(Vector2(300.f,poses[i]) * matInBackGround);
		_buttons[i]->RegistParent(_backGround);
	}

	//カーソルを移動
	_carsol->SetFrom(_carsol->GetLocation());
	_carsol->SetTo(_buttons[_buttonNum]->GetWorldLocation() * _inMatCarsolParent);
	_carsol->SetFrameCount(0.f);
	RegistUI(_carsol);

	for (size_t i = 0; i < _BGMLevelUIs.size(); ++i)
	{
		_BGMLevelUIs[i] = NEW Graph(this, 0);
		_BGMLevelUIs[i]->Load("res/UI/Select/SoundAdjust/ui_volume_control_01.png");
		_BGMLevelUIs[i]->SetWidth(50.f + 12.f * i);
		_BGMLevelUIs[i]->SetHeight(50.f + 12.f * i);
		_BGMLevelUIs[i]->SetLeftLocation(Vector2(900.f + 132.f * (float)i, 228.f - 12.f * (float)i) * matInBackGround);
		_BGMLevelUIs[i]->RegistParent(_backGround);
	}

	for (size_t i = 0; i < _SELevelUIs.size(); ++i)
	{
		_SELevelUIs[i] = NEW Graph(this, 0);
		_SELevelUIs[i]->Load("res/UI/Select/SoundAdjust/ui_volume_control_01.png");
		_SELevelUIs[i]->SetWidth(50.f + 12.f * i);
		_SELevelUIs[i]->SetHeight(50.f + 12.f * i);
		_SELevelUIs[i]->SetLeftLocation(Vector2(900.f + 132.f * (float)i, 408 - 12.f * (float)i) * matInBackGround);
		_SELevelUIs[i]->RegistParent(_backGround);
	}

	//背景を画面下に
	{
		int screenH = ApplicationBase::GetInstance()->DispSizeH();

		_backGround->SetLeftLocation(100.f, 1040.f + (float)screenH);
		_backGround->SetScale(0.f, 0.f);
	}
	//アニメーションの移動量
	Vector2 to(180.f + _backGround->GetWidth() / 2.f, 60.f + _backGround->GetHeight() / 2.f);
	Vector2 diff = to - _backGround->GetLocation();

	CreateScaleAnim("Scale", 1.f, 1.f, 20);
	CreateLocationAnim("Scale", diff.x, diff.y, 20, Easing::OUT_QUART);

	_backGround->PlayAnimation("Scale");

	CreateRotateAnim("Lean", PI / 12.f, 5);

	_buttons.front()->PlayAnimation("Lean");
}

SettingScreen::~SettingScreen()
{
	RemoveUI(_carsol);
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

		_carsol->SetFrom(_carsol->GetLocation());
		_carsol->SetTo(_buttons[_buttonNum]->GetWorldLocation()* _inMatCarsolParent);
		_carsol->SetFrameCount(0.f);
	}
	if (global._trg & PAD_INPUT_DOWN)
	{
		_buttons[_buttonNum]->ReverseAnimation();
		_buttonNum = _buttonNum == _buttons.size() - 1 ? _buttons.size() - 1 : _buttonNum + 1;
		_buttons[_buttonNum]->PlayAnimation("Lean");

		_carsol->SetFrom(_carsol->GetLocation());
		_carsol->SetTo(_buttons[_buttonNum]->GetWorldLocation()* _inMatCarsolParent);
		_carsol->SetFrameCount(0.f);
	}

	if (global._trg & PAD_INPUT_1) {
		switch (_buttonNum)
		{
		case(BGM):
		{

			NEW SoundAdjustScreen(GetOwner(), _carsol, &_BGMLevelUIs,SoundItemBase::TYPE::BGM);
			break;
		};

		case(SE):
		{			
			NEW SoundAdjustScreen(GetOwner(), _carsol, &_SELevelUIs, SoundItemBase::TYPE::SE);
			break;
		};

		case(BACK):
		{
			_carsol->SetFrom(_carsol->GetLocation());
			_carsol->SetFrameCount(0.f);
			_carsol->SetTo(_backPos);
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
		_carsol->SetFrom(_carsol->GetLocation());
		_carsol->SetFrameCount(0.f);
		_carsol->SetTo(_backPos);
		_backGround->ReverseAnimation();
		_isFinish = true;
	}
}