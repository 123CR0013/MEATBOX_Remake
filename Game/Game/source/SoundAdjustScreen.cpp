#include"SoundAdjustScreen.h"
#include"MoveUI.h"

constexpr float TAKE_FRAME = 5.f;

SoundAdjustScreen::SoundAdjustScreen(ModeUI* owner, MoveUI* carsol, std::array<class Box*, 5>* BGMLevelUIs,SoundItemBase::TYPE type)
	:UIScreen(owner)
	,_carsol(carsol)
	,_LevelUIs(BGMLevelUIs)
	,_backPos(carsol->GetLocation())
	,_type(type)
{

	if (carsol->GetParent())
	{
		_inWorldMatParent = Matrix3::CreateTranslation(carsol->GetParent()->GetLocation()).Invert();
	}
	else
	{
		_inWorldMatParent = Matrix3::Identity;
	}

	_level = global._soundServer->GetVolume(type) / (255 / _LevelUIs->size());

	_level = (unsigned int)MyMath::Clamp(0.f, (float)(_LevelUIs->size() - 1), (float)_level);

	_carsol->SetFrom(_carsol->GetLocation());
	_carsol->SetTo((*_LevelUIs)[_level]->GetWorldLocation() * _inWorldMatParent);
	_carsol->SetFrameCount(0.f);

	CreateScaleAnim("plus", 1.f,0.f, 10);
	CreateScaleAnim("minus", -1.f, 0.f, 10);

	//操作説明UI
	{
		_tutorialArrow = NEW Graph(this);
		_tutorialArrow->Load("res/UI/ui_tutorial_arrow_02.png");
		_tutorialArrow->SetLeftLocation(30.f, 997.f);

		_tutorialSelect = NEW Graph(this);
		_tutorialSelect->Load("res/UI/ui_tutorial_select_01.png");
		_tutorialSelect->SetLeftLocation(184.f, 997.f);

		_tutorialReturn = NEW Graph(this);
		_tutorialReturn->Load("res/UI/ui_tutorial_return_01.png");
		_tutorialReturn->SetLeftLocation(993.f, 997.f);
	}

	RegistUI(_carsol);
}

SoundAdjustScreen::~SoundAdjustScreen()
{
	RemoveUI(_carsol);

	std::string outData = std::to_string(global._soundServer->GetSEVolume()) + "," + std::to_string(global._soundServer->GetBGMVolume()) + "," + std::to_string(global._soundServer->GetVOICEVolume());
	
	CFile volumeFile("data/Volume.txt",outData);
}

void SoundAdjustScreen::Update()
{
	UIScreen::Update();

	if (global._trg & PAD_INPUT_LEFT)
	{
		if (_level > 0)
		{
			(*_LevelUIs)[_level]->PlayAnimation("minus");

			_level =  _level - 1;
			_carsol->SetFrom(_carsol->GetLocation());
			_carsol->SetTo((*_LevelUIs)[_level]->GetWorldLocation() * _inWorldMatParent);
			_carsol->SetFrameCount(0.f);

			global._soundServer->ChangeVolume(_type, (float)(_level) / (float)_LevelUIs->size());

			if (_type == SoundItemBase::TYPE::SE) { 
				global._soundServer->Play("SE_06");
			}

			global._soundServer->Play("SE_01");
		}
	}
	if (global._trg & PAD_INPUT_RIGHT)
	{
		if (_level < (*_LevelUIs).size() - 1) {
			_level = _level + 1;

			_carsol->SetFrom(_carsol->GetLocation());
			_carsol->SetTo((*_LevelUIs)[_level]->GetWorldLocation() * _inWorldMatParent);
			_carsol->SetFrameCount(0.f);

			(*_LevelUIs)[_level]->PlayAnimation("plus");

			global._soundServer->ChangeVolume(_type, (float)(_level)/ (float)_LevelUIs->size());

			if (_type == SoundItemBase::TYPE::SE) { 
				global._soundServer->Play("SE_06");
			}
		}

		global._soundServer->Play("SE_01");
	}

	//Zボタンで戻る
	if (global._trg & PAD_INPUT_1)
	{
		_carsol->SetFrom(_carsol->GetLocation());
		_carsol->SetTo(_backPos);
		_carsol->SetFrameCount(0.f);
		DeleteUIScreen(this);
		global._soundServer->Play("SE_04");
	}

	//XボタンでSEを鳴らす,デバッグ用
	if (global._trg & PAD_INPUT_3)
	{
		global._soundServer->Play("SE_06",255);
	}
}