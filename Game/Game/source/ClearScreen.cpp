#include"ClearScreen.h"
#include"ModeSelect.h"

ClearScreen::ClearScreen(ModeUI* owner)
	:UIScreen(owner)
{
	_backGround = NEW Graph(this);
	_backGround->Load("res/Demo/demo_info_02.png");
	_backGround->SetLocation(0.f,-1920.f);
	_backGround->SetDrawType(UI::DrawType::kLeft);

	Text* text = NEW Text(this);
	text->SetSize(50.f * 8.f, 50.f);
	text->SetTextSize(50.f);
	text->SetText("Press Z");
	text->RegistParent(_backGround);
	text->SetDrawType(UI::DrawType::kLeft);

	CreateLocationYAnim("Drop", 1920.f, 120, Easing::OUT_BOUNCE);

	_backGround->PlayAnimation("Drop");
}

ClearScreen::~ClearScreen(){}

void ClearScreen::Update()
{
	UIScreen::Update();

	if (_backGround->IsFinishAnimation())
	{
		if (global._trg & PAD_INPUT_1)
		{
			ModeServer::GetInstance()->Del(GetOwner());
			ModeServer::GetInstance()->Add(NEW ModeSelect(), 1, "ModeSelect");
		}
	}
}