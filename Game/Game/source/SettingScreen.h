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
	Matrix3 _inMatCarsolParent;	//�J�[�\���̐e�I�u�W�F�N�g�̈ʒu�̋t�s��	
	const Vector2 _backPos;		//�ݒ��ʂ��I�������ۂɃJ�[�\�����߂���W

	bool _isFinish;				//�ݒ��ʂ̏I���t���O
};