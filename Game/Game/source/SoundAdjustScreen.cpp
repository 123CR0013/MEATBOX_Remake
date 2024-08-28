#include"SoundAdjustScreen.h"
#include"MoveUI.h"

SoundAdjustScreen::SoundAdjustScreen(ModeUI* owner, MoveUI* select, std::array<class Box*, 5>* BGMLevelUIs, const Vector2& backPos,SoundItemBase::TYPE type)
	:UIScreen(owner)
	,_select(select)
	,_LevelUIs(BGMLevelUIs)
	,_backPos(backPos)
	,_type(type)
{
	_level = global._soundServer->GetVolume(type) / (255 / _LevelUIs->size());

	_level = (unsigned int)MyMath::Clamp(0.f, (float)(_LevelUIs->size() - 1), (float)_level);

	_select->SetFrom(_select->GetLocation());
	_select->SetTo((*_LevelUIs)[_level]->GetLocation());
	_select->SetFrameCount(20.f);

	RegistUI(_select);
}

SoundAdjustScreen::~SoundAdjustScreen()
{
	RemoveUI(_select);

	std::string outData = std::to_string(global._soundServer->GetSEVolume()) + "," + std::to_string(global._soundServer->GetBGMVolume()) + "," + std::to_string(global._soundServer->GetVOICEVolume());
	
	CFile volumeFile("GameParam/Volume.txt",outData);
}

void SoundAdjustScreen::Update()
{
	UIScreen::Update();

	if (global._trg & PAD_INPUT_LEFT)
	{
		if (_level > 0)
		{
			//(*mLevelUIs)[mLevel]->ReverseAnimation();
			_level =  _level - 1;
			_select->SetFrom(_select->GetLocation());
			_select->SetTo((*_LevelUIs)[_level]->GetLocation());
			_select->SetFrameCount(20.f);

			global._soundServer->ChangeVolume(_type, (float)(_level + 1) / (float)_LevelUIs->size());

			if (_type == SoundItemBase::TYPE::SE) { 
				global._soundServer->Play("SE_06");
			}
		}
	}
	if (global._trg & PAD_INPUT_RIGHT)
	{
		if (_level < (*_LevelUIs).size() - 1) {
			_level = _level + 1;

			_select->SetFrom(_select->GetLocation());
			_select->SetTo((*_LevelUIs)[_level]->GetLocation());
			_select->SetFrameCount(20.f);

			global._soundServer->ChangeVolume(_type, (float)(_level + 1)/ (float)_LevelUIs->size());

			if (_type == SoundItemBase::TYPE::SE) { 
				global._soundServer->Play("SE_06");
			}
		}
	}

	//Zボタンで戻る
	if (global._trg & PAD_INPUT_1)
	{
		_select->SetFrom(_select->GetLocation());
		_select->SetTo(_backPos);
		_select->SetFrameCount(20.f);
		DeleteUIScreen(this);
	}

	//XボタンでSEを鳴らす,デバッグ用
	if (global._trg & PAD_INPUT_2)
	{
		global._soundServer->Play("SE_06",255);
	}
}