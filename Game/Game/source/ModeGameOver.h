#pragma once
#include "appframe.h"

// ���[�h
class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

protected:
	int _bgGraphHandle;
};