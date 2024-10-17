#include "ModeGameOver.h"
#include "ModeGame.h"

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
        ModeServer::GetInstance()->Add(NEW ModeGame(0,0), 1, "ModeGame");
	}

    return true;
}

bool ModeGameOver::Render()
{
    DrawGraph(0, 0, _bgGraphHandle, TRUE);
    return true;
}
