#pragma once
#include"appframe.h"

class ModeSelect :public ModeUI {
public:
	ModeSelect();
	virtual ~ModeSelect();

	bool Initialize()override;
private:
	class StageSelectScreen* _screen;
};