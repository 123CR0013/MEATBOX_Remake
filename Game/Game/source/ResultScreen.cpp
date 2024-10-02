#include"ResultScreen.h"
#include"ModeGame.h"

ResultScreen::ResultScreen(ModeUI* owner)
	:UIScreen(owner)
{
	ZFile ifs("data/StageData/1.amg");

	Cell data;
	CSVFile::Parse(ifs.DataStr(),CSVFile::Type::LINE, data);

	_score = 0;
	size_t i = 0;
	for (i = 0; i < data["Score"].size(); ++i)
	{
		if (data["Score"][i].GetInt() > _score)break;
	}

	std::string drawScore[] = { "D","C","B","A" };

	{
		Text* text = NEW Text(this);
		text->SetText(drawScore[i]); 
		text->SetWidth(50.f);
		text->SetHeight(50.f);
		text->SetLocation(25.f, 25.f);
	}

	for (i = 0; i < data["Score"].size(); ++i)
	{
		Text* text = NEW Text(this);
		text->SetText(data["Score"][i].GetStr());
		text->SetWidth(50.f);
		text->SetHeight(50.f);
		text->SetLocation(25.f, 75.f + (float)i * 50.f);
	}
}

ResultScreen::~ResultScreen()
{
}

void ResultScreen::Update()
{
	UIScreen::Update();
}