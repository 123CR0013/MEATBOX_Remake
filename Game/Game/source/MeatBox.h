#pragma once
#include "GameObject.h"
class StickyGroup;

class MeatBox : public GameObject
{
public:
	MeatBox(ModeBase* mode);
	~MeatBox() override;
	
	// �ړ��ł��邩�ǂ���
	// �N���X�O������Ăяo���C���^�[�t�F�[�X
	bool CheckMove(Vector3 vMove) override;

	// �ړ��ł��邩�ǂ���
	// �N���X�����A���́AStickyGroup�Ŏg�p����֐�
	bool _CheckMove(Vector3 vMove);

	// �ړ�����
	void Move(Vector3 vMove);

	void DrawDebug() override;

	StickyGroup* GetStickyGroup() { return _pStickyGroup; }
	// StickyGroup�N���X���Ŏg�p���邱��
	void SetStickyGroup(StickyGroup* pSticky) { _pStickyGroup = pSticky; } 

protected:
	MapChip* CheckMapChip(Vector3 vPos);
	GameObject* CheckObject(Vector3 vPos);


	StickyGroup* _pStickyGroup;
	void SetExistHole(bool bExistHole);
	void SetExistFloor(bool bExistFloor);
	void SetGoThroughHole(bool bGoThroughHole);
};