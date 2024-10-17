#pragma once
#include"appframe.h"

class StageSelectScreen :public UIScreen {
public:
	StageSelectScreen(class ModeUI* owner);
	virtual ~StageSelectScreen();

	void Update()override;
	void Draw()override;

private:

	std::array<class StageSelectButton*,5>_selectButtons;

	//ステージ種類の配列番号
	unsigned int _stageTypeNum;

	//ステージの配列番号
	unsigned int _stageNum;

	//UIのスライドする際のイージングで使用
	Vector2 _pos;
	Vector2 _from;
	Vector2 _to;

	float _frameCount;
	float _takeFrame;
	unsigned int _fromFrame;

	Graph* _container;
	Graph* _meatBox;
	Graph* _terminal;

	FlipBook* _light;
	FlipBook* _arm;

	Empty* _containerParent;
	Empty* _terminalParent;
	Empty* _armParent;

	char _moveArrowX;
	bool _isSelect;
};