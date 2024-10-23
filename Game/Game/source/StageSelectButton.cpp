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

	_backGround = NEW Graph(GetOwner(),0);
	_backGround->RegistParent(this);
	_backGround->SetWidth(1920.f);
	_backGround->SetHeight(1080.f);
	_backGround->Load("res/UI/Select/Stage/ui_stage_background_03.png");

	const Matrix3 inMat = Matrix3::CreateTranslation(_transform.mLocation).Invert();

	_monitor = NEW Graph(GetOwner(),500);
	_monitor->RegistParent(this);
	_monitor->SetLocation(Vector2(966.f + 1920.f * static_cast<float>(_stageNum), 447.f) * inMat);
	_monitor->Load("res/UI/Select/Stage/ui_stage_monitor_01.png");

	//ƒ{ƒ^ƒ“ì¬
	{
		Matrix3 monitorInMat = Matrix3::CreateTranslation(_monitor->GetWorldLocation()).Invert();
		Vector2 pos[BUTTON_NUM] =
		{
			{675.f,551.f},
			{820.f,551.f},
			{965.f,551.f},
			{1110.f,551.f},
			{1255.f,551.f}
		};
		for (size_t i = 0; i < BUTTON_NUM; ++i)
		{
			_buttons[i] = NEW Graph(GetOwner(), 501);
			_buttons[i]->Load("res/UI/Select/Stage/ui_stage_folder_01.png");
			_buttons[i]->RegistParent(_monitor);
			_buttons[i]->SetLocation(Vector2(pos[i].x + 1920.f * static_cast<float>(_stageNum), pos[i].y) * monitorInMat);
		}
	}
	std::string fileName = "data/StageData/" + std::to_string(_stageNum + 1) + ".amg";

	ZFile ifs(fileName);

	if (ifs.Success())
	{
		CSVFile::Cell data;
		CSVFile::Parse(ifs.DataStr(), CSVFile::Type::LINE, data);

		for (size_t i=0; i< data["Button"].size();++i)
		{
			_stageDatas[i] = data["Button"][i].GetBool();

			if (!_stageDatas[i])_buttons[i]->Load("res/UI/Select/Stage/ui_stage_folder_02.png");
		}
	}
	else
	{
		std::string data = "“WŠJŽ¸”s:" + ifs.Filename();
		MessageBoxA(NULL, data.c_str(), NULL, MB_OK);
	}
}

StageSelectButton::~StageSelectButton()
{
	std::string fileName = "data/StageData/" + std::to_string(_stageNum + 1) + ".amg";

	ZFile ifs(fileName);

	if (ifs.Success())
	{
		CSVFile::Cell data;
		CSVFile::Parse(ifs.DataStr(), CSVFile::Type::LINE, data);

		for(size_t i=0;i<_stageDatas.size();++i)
		{
			data["Button"][i] = _stageDatas[i];
		}

		std::string fileName = "data/StageData/" + std::to_string(_stageNum + 1) + ".amg";
		ZFile save(fileName, CSVFile::Format(data,CSVFile::Type::LINE));
	}
}
