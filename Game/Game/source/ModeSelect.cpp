#include"ModeSelect.h"
#include"StageSelectScreen.h"
#include"TitleScreen.h"

ModeSelect::ModeSelect() 
{

}

ModeSelect::~ModeSelect() {
}

bool ModeSelect::Initialize()
{
	ModeUI::Initialize();

	NEW TitleScreen(this);
	return true;
}