#pragma once
#include"Appframe.h"

class SoundAdjustScreen  :public UIScreen
{
public:
	SoundAdjustScreen(class ModeUI* owner,class MoveUI* select,std::array<class Box*,5>* BGMLevelUIs,const Vector2& backPos, SoundItemBase::TYPE type);
	~SoundAdjustScreen();

	void Update()override;
private:
	std::array<class Box*, 5>*_LevelUIs;
	unsigned int _level;

	const Vector2 _backPos;

	class MoveUI* _select;

	SoundItemBase::TYPE _type;
};

