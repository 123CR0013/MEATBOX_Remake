#pragma once
#include"Appframe.h"

class SoundAdjustScreen  :public UIScreen
{
public:
	SoundAdjustScreen(class ModeUI* owner,class MoveUI* select,std::array<class Box*,5>* BGMLevelUIs, SoundItemBase::TYPE type);
	~SoundAdjustScreen();

	void Update()override;
private:
	std::array<class Box*, 5>*_LevelUIs;
	unsigned int _level;

	const Vector2 _backPos;		

	class MoveUI* _carsol;
	Matrix3 _inWorldMatParent;

	SoundItemBase::TYPE _type;


	//ëÄçÏê‡ñæUI
	Graph* _tutorialArrow;
	Graph* _tutorialSelect;
	Graph* _tutorialReturn;
};

