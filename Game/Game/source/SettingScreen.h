#pragma once
#include"appframe.h"

class SettingScreen :public UIScreen
{
public:
	SettingScreen(class MoveUI* carsol,class ModeUI* owner);
	virtual ~SettingScreen();

	void Update()override;

private:
	unsigned int _buttonNum;
	class Graph* _backGround;
	std::array<class Graph*, 3>_buttons;
	std::array<class Graph*, 5>_BGMLevelUIs;
	std::array<class Graph*, 5>_SELevelUIs;

	unsigned int _BGMLevel;
	unsigned int _SELevel;

	class MoveUI* _carsol;
	Matrix3 _inMatCarsolParent;	//カーソルの親オブジェクトの位置の逆行列	
	const Vector2 _backPos;		//設定画面が終了した際にカーソルが戻る座標

	bool _isFinish;				//設定画面の終了フラグ
};