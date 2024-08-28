#include"StageSelectScreen.h"
#include"StageSelectButton.h"
#include"MenuScreen.h"

constexpr float TAKE_FRAME = 30.f;

StageSelectScreen::StageSelectScreen(ModeUI* owner)
	:UIScreen(owner)
	,_frameCount(0)
	,_fromFrame(0)
	,_isSelect(false)
	,_stageTypeNum(0)
	,_stageNum(0)
{
	for (size_t i = 0;i<_selectButtons.size();++i) 
	{
		_selectButtons[i] = NEW StageSelectButton(this,i);
	}

	_pos = _selectButtons.front()->GetLocation();
	_from = _pos;

	CreateLocationAnim("BackButtonMove", 120.f, 0.f, 10);
	CreateScaleAnim("Select", 0.5f, 0.5f, 5);
}

StageSelectScreen::~StageSelectScreen(){}

void StageSelectScreen::Update() {
	UIScreen::Update();

	if (_isSelect)
	{
		if (global._trg & PAD_INPUT_3)
		{
			_selectButtons[_stageTypeNum]->ReverseAnimation();
			_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->ReverseAnimation();
			_stageNum = 0;
			_isSelect = false;
		}

		for (size_t i = 0; i < _selectButtons[_stageTypeNum]->GetButtons().size(); ++i)
		{
			if (_selectButtons[_stageTypeNum]->GetButtons()[i]->IsRightClickTrg())
			{
				if (_selectButtons[_stageTypeNum]->GetStageDatas()[i] == "FALSE")
				{
					_selectButtons[_stageTypeNum]->GetStageDatas()[i] = "TRUE";
					_selectButtons[_stageTypeNum]->GetButtons()[i]->SetColor(0, 255, 255);
				}
				else if(i != _stageNum)
				{
					_selectButtons[_stageTypeNum]->GetStageDatas()[i] = "FALSE";
					_selectButtons[_stageTypeNum]->GetButtons()[i]->SetColor(255, 255, 0);
				}
			}
		}

		if (global._trg & PAD_INPUT_LEFT)
		{
			_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->ReverseAnimation();
			_stageNum = _stageNum == 0 ? 0 : _stageNum - 1;

			//そのステージがプレイできるか
			if (_selectButtons[_stageTypeNum]->GetStageDatas()[_stageNum] != "TRUE")_stageNum++;

			_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->PlayAnimation("Select");
		}

		if (global._trg & PAD_INPUT_RIGHT)
		{
			_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->ReverseAnimation();
			size_t size = _selectButtons[_stageTypeNum]->GetButtons().size();
			_stageNum = _stageNum == size - 1 ? size - 1 : _stageNum + 1;

			//そのステージがプレイできるか
			if (_selectButtons[_stageTypeNum]->GetStageDatas()[_stageNum] != "TRUE")_stageNum--;
			
			_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->PlayAnimation("Select");
		}
	}
	else
	{
		if (global._trg & PAD_INPUT_LEFT)
		{
			_stageTypeNum = _stageTypeNum == 0 ? 0 : _stageTypeNum - 1;
			_to = _selectButtons[_stageTypeNum]->GetLocation();
			_frameCount =  TAKE_FRAME;
		}

		if (global._trg & PAD_INPUT_RIGHT)
		{
			_stageTypeNum = _stageTypeNum == _selectButtons.size() - 1 ? _selectButtons.size() - 1 : _stageTypeNum + 1;
			_to = _selectButtons[_stageTypeNum]->GetLocation();
			_frameCount =  TAKE_FRAME;
		}

		if (global._trg & PAD_INPUT_1)
		{
			_selectButtons[_stageTypeNum]->PlayAnimation("Select");
			_selectButtons[_stageTypeNum]->GetButtons()[_stageNum]->PlayAnimation("Select");
			_isSelect = true;
		}
		if (global._trg & PAD_INPUT_9)
		{
			NEW MenuScreen(GetOwner());
		}
	}

	if (_frameCount > 0.f)
	{
		_frameCount -= 1.f;
		
		_pos = Vector2::Lerp(_from, _to, 1.f - _frameCount / TAKE_FRAME);
		auto beforePos = Vector2::Lerp(_from, _to, 1.f - (_frameCount + 1.f) / TAKE_FRAME);

		for (auto&& buttons : _selectButtons)
		{
			buttons->SetLocation(buttons->GetLocation() - (_pos - beforePos));
		}
	}
	else
	{
		_pos = _from;
	}
}

void StageSelectScreen::Draw()
{
	UIScreen::Draw();
}