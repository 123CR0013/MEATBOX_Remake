#include"StageSelectButton.h"
#include"appframe.h"

StageSelectButton::StageSelectButton(UIScreen* owner,size_t stageNum)
	:Empty(owner)
	,_stageNum(stageNum)
{
	auto apliInst = ApplicationBase::GetInstance();
	float dispSizeW = static_cast<float>(apliInst->DispSizeW());

	_width = 700.f;
	_height = 700.f;
	_transform.mLocation = Vector2(dispSizeW / 2.f + dispSizeW * static_cast<float>(_stageNum), 300.f);

	_backGround = NEW Box(GetOwner());
	_backGround->RegistParent(this);
	_backGround->SetWidth(700.f);
	_backGround->SetHeight(700.f);
	_backGround->SetColor(255, 255, 255);

	for (size_t i = 0; i < _buttons.size(); ++i)
	{
		_buttons[i] = NEW Button(GetOwner());
		_buttons[i]->RegistParent(_backGround);
		_buttons[i]->SetWidth(100.f);
		_buttons[i]->SetHeight(50.f);
		_buttons[i]->SetLocation(-100.f + (100.f + 50.f) * i, 0.f);
	}

	std::string fileName = "data/StageData/" + std::to_string(_stageNum + 1) + ".amg";

	ZFile csv(fileName);

	if (csv.Success())
	{
		size_t pos = 0;

		for (size_t i = 0; i < BUTTON_NUM; ++i)
		{
			size_t to = csv.DataStr().substr(pos).find(",");
			_stageDatas[i] = csv.DataStr().substr(pos, to);
			pos += to + 1;

			if (_stageDatas[i] == "TRUE")
			{
				_buttons[i]->SetColor(0, 255, 255);
			}
			else
			{
				_buttons[i]->SetColor(255, 255, 0);
			}
		}
	}
	else
	{
		std::string data = "展開失敗:" + csv.Filename();
		MessageBoxA(NULL, data.c_str(), NULL, MB_OK);
	}
	
	{
		_stageName = NEW Text(GetOwner());
		_stageName->RegistParent(this);
		_stageName->SetText("ステージ" + std::to_string(_stageNum));
		_stageName->SetWidth(200.f);
		_stageName->SetHeight(100.f);
		_stageName->SetLocation(0.f, -100.f);
	}
}

StageSelectButton::~StageSelectButton()
{
	std::string data;

	for (size_t i = 0; i < BUTTON_NUM; ++i)
	{
		data += _stageDatas[i] + ",";
	}

	std::string fileName = "res/StageData/" + std::to_string(_stageNum + 1) + ".csv";
	CFile csv(fileName,data);
}
