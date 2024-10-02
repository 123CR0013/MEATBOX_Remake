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
	_transform.mLocation = Vector2(1920.f / 2.f + 1920.f * static_cast<float>(_stageNum), 1080.f / 2.f);

	_backGround = NEW Box(GetOwner());
	_backGround->RegistParent(this);
	_backGround->SetWidth(1920.f);
	_backGround->SetHeight(1080.f);
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

	ZFile ifs(fileName);

	if (ifs.Success())
	{
		Cell data;
		CSVFile::Parse(ifs.DataStr(), CSVFile::Type::LINE, data);

		for (size_t i=0; i< data["Button"].size();++i)
		{
			_stageDatas[i] = data["Button"][i].GetBool();

			if (_stageDatas[i])
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
		std::string data = "展開失敗:" + ifs.Filename();
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
	std::string fileName = "data/StageData/" + std::to_string(_stageNum + 1) + ".amg";

	ZFile ifs(fileName);

	if (ifs.Success())
	{
		Cell data;
		CSVFile::Parse(ifs.DataStr(), CSVFile::Type::LINE, data);

		for(size_t i=0;i<_stageDatas.size();++i)
		{
			data["Button"][i] = _stageDatas[i];
		}

		std::string fileName = "data/StageData/" + std::to_string(_stageNum + 1) + ".amg";
		ZFile save(fileName, CSVFile::Format(data,CSVFile::Type::LINE));
	}
}
