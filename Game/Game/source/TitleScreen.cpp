#include"TitleScreen.h"
#include"StageSelectScreen.h"

TitleScreen::TitleScreen(ModeUI* owner)
	:UIScreen(owner)
{
	_backGround = NEW Graph(this);
	_backGround->Load("res/Demo/demo_info_01.png");
	_backGround->SetDrawType(UI::DrawType::kLeft);

	_fadeBox = NEW Box(this, 500000);
	_fadeBox->SetDrawType(UI::DrawType::kLeft);
	_fadeBox->SetColor(0, 0, 0);
	_fadeBox->SetSize(1920.f, 1280.f);

	CreateOpacityAnim("FadeIn", -1.f, 60);
	CreateOpacityAnim("FadeOut", 1.f, 60);

	_fadeBox->PlayAnimation("FadeIn");
}

TitleScreen::~TitleScreen()
{

}

void TitleScreen::Update()
{
	UIScreen::Update();

	if (_fadeBox->IsFinishAnimation())
	{
		if (global._trg & PAD_INPUT_1)
		{
			_fadeBox->PlayAnimation("FadeOut");

			std::function<void()> exe = [=]()mutable {DeleteUIScreen(this); NEW StageSelectScreen(GetOwner()); };
			ModeTimeTable::Add(exe, 60);
		}
	}

}