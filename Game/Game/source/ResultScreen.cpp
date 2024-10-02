#include"ResultScreen.h"
#include"ModeGame.h"

ResultScreen::ResultScreen(ModeUI* owner)
	:UIScreen(owner)
	,_frameCount(0)
	,_eleNum(0)
{
	ZFile ifs("data/StageData/1.amg");

	CSVFile::Cell data;
	CSVFile::Parse(ifs.DataStr(),CSVFile::Type::LINE, data);

	_score = 31;
	size_t i = 0;
	for (i = 0; i < data["Score"].size(); ++i)
	{
		if (data["Score"][i].GetInt() >= _score) break;
	}

	_eleNum = (int)i;

	for (i = 0; i < data["Score"].size(); ++i)
	{
		Text* text = NEW Text(this);
		text->SetText(data["Score"][i].GetStr());
		text->SetWidth(50.f);
		text->SetHeight(50.f);
		text->SetLocation(25.f, 500.f + (float)i * 50.f);
	}

	CreateLocationXAnim("Slide", -1920.f / 2.f, 30.f, Easing::LINEAR);


	_scoreText = NEW Text(this);
	_scoreText->SetText("0");
	_scoreText->SetWidth(50.f);
	_scoreText->SetHeight(50.f);
	_scoreText->SetLocation(500.f, 500.f);



}

ResultScreen::~ResultScreen()
{
}

void ResultScreen::Update()
{
	UIScreen::Update();

	if (_frameCount++ < 60)
	{
		int draw = static_cast<int>(Easing::Linear((float)_frameCount, 0.f, (float)_score, (float)60));
		_scoreText->SetText(std::to_string(draw));

		if (_frameCount == 60)
		{
			std::string drawScore[] = { "D","C","B","A" };
			std::string typeName[] = { "ï‡êî","åÇîjêî","ÉâÉìÉN:" + drawScore[_eleNum] };
			for (size_t i = 0; i < 3; ++i)
			{
				Text* text = NEW Text(this);
				text->SetText(typeName[i]);
				text->SetWidth(300.f);
				text->SetHeight(50.f);
				text->SetLocation(2070.f, 300.f + (float)i * 50.f);
				text->SetTextSize(20.f);

				std::function<void()>exe = [=]()mutable {text->PlayAnimation("Slide"); };
				ModeTimeTable::Add(exe, 30 * i);
			}
		}
	}
}