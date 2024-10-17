#include"ModeSelect.h"
#include"StageSelectScreen.h"
#include"ResultScreen.h"

ModeSelect::ModeSelect() 
{

}

ModeSelect::~ModeSelect() {
}

bool ModeSelect::Initialize()
{
	ModeUI::Initialize();

	global._soundServer->Play("BGM_01");

	NEW StageSelectScreen(this);
	return true;
}