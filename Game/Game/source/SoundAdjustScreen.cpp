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
	}

	//Zボタンで戻る
	if (global._trg & PAD_INPUT_1)
	{
		_carsol->SetFrom(_carsol->GetLocation());
		_carsol->SetTo(_backPos);
		_carsol->SetFrameCount(0.f);
		DeleteUIScreen(this);
	}

	//XボタンでSEを鳴らす,デバッグ用
	if (global._trg & PAD_INPUT_2)
	{
		global._soundServer->Play("SE_06",255);
	}
}