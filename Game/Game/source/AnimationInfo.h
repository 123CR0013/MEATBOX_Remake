#pragma once
#include "appframe.h"
#include <vector>

class AnimationInfo
{
public:
	AnimationInfo();
	~AnimationInfo();

	// �摜�n���h��
	std::vector<int> _graphHandle;
	// �`�揇�e�[�u��
	std::vector<int> _drawTbl;
	// �e�[�u���̗v�f��
	int _tblNum;
	// 1��������̕\���t���[����
	int _framePerSheet;
};