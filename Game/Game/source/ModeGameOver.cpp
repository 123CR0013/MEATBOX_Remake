#include "ModeGameOver.h"
#include "ModeGame.h"

ModeGameOver::ModeGameOver(ModeBase* nextMode)
{
	_bgGraphHandle = -1;
	_nextMode = nextMode;
}

bool ModeGameOver::Initialize()
{
    _bgGraphHandle = ResourceServer::LoadGraph("res/UI/GameOver/GameOver.png");
    return true;
}

bool ModeGameOver::Terminate()
{
    return true;
}

bool ModeGameOver::Process()
{
    if(global._trg & PAD_INPUT_1) {
		ModeServer::GetInstance()->Del(this);

		_nextMode->ResetStage();
		_nextMode->SetPause(false);
	}

    return true;
}

bool ModeGameOver::Render()
{
    DrawGraph(0, 0, _bgGraphHandle, TRUE);
    return true;
}
