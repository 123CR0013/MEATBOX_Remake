#pragma once
#include "appframe.h"

// ÉÇÅ[Éh
class ModeGameOver : public ModeBase
{
	typedef ModeBase base;
public:
	ModeGameOver(ModeBase* nextMode);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

protected:
	int _bgGraphHandle;
	ModeBase* _nextMode;
};