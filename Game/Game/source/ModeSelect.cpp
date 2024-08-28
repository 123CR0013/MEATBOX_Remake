#include"ModeSelect.h"
#include"StageSelectScreen.h"

ModeSelect::ModeSelect() 
{

}

ModeSelect::~ModeSelect() {

}

bool ModeSelect::Initialize()
{
	ModeUI::Initialize();

	global._soundServer->Play("BGM_01");

	_screen = NEW StageSelectScreen(this);
	return true;
}