#include "StickyGroup.h"
#include "Sticky.h"
#include "MeatBox.h"
#include "Effect.h"

int tmpId = 0;

StickyGroup::StickyGroup(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::STICKYGROUP;
	_bSetToMap = false;

	_bExistHole = false;
	_bExistFloor = false;
	_bGoThroughHole = false;

	id = tmpId;
	tmpId++;
}

StickyGroup::~StickyGroup()
{
	_pSticky.clear();
	_pMeatBox.clear();
	_pDeleteEnemyList.clear();
}

bool StickyGroup::CheckMove(Vector3 vMove)
{
	// �ȉ���3�̃t���O�̓~�[�g�{�b�N�X��_CheckMove�֐����ōX�V�����

	// �O���[�v���̃~�[�g�{�b�N�X�̈ړ���Ɍ������邩
	_bExistHole = false;
	// �O���[�v���̃~�[�g�{�b�N�X�̈ړ���ɏ������邩
	_bExistFloor = false;
	// ����ʂ蔲�����邩
	// �~�[�g�{�b�N�X��_CheckMove�֐����ň�x�ł�false�ɂȂ�����A���̃t���O��false�̂܂�
	_bGoThroughHole = true;

	// ���̃O���[�v���̃~�[�g�{�b�N�X�����ׂĈړ��ł��邩�ǂ����𒲂ׂ�
	bool bCanMove = true;
	for(auto pMB : _pMeatBox)
	{
		bCanMove = pMB->_CheckMove(vMove) && bCanMove;
	}

	// �ȉ��̂ǂ��炩�̏ꍇ�Ɉړ��ł���
	// �@���ׂẴ~�[�g�{�b�N�X���ړ��ł���ꍇ
	// �A�O���[�v���̃~�[�g�{�b�N�X�̈ꕔ���ړ��\�ł���A���𒴂�����ꍇ
	bool bResult = bCanMove || (_bExistHole && _bExistFloor && _bGoThroughHole);
	if (bResult)
	{
		// �~�[�g�{�b�N�X���ړ�
		for(auto pMB : _pMeatBox)
		{
			pMB->Move(vMove);
		}

		// �����Ԃ���G���폜
		int num = 0;
		for (auto pEnemy : _pDeleteEnemyList)
		{
			// 5�t���[�����Ƃɔj��G�t�F�N�g���Đ�
			Vector3 vEnPos = pEnemy->GetPos();
			std::function<void()> func = [=]() {CreateEffect_Explosion(vEnPos, _mode, num); };
			ModeTimeTable::Add(func, 5 * num);
			num++;

			pEnemy->Destroy();
		}
	}

	_pDeleteEnemyList.clear();

	return bResult;
}

// ����StickyGroup�ƌ�������
void StickyGroup::Merge(StickyGroup* other)
{
	if (other == nullptr || this == other) return;

	// ��������StickyGroup���X�V
	for (auto pSticky : other->_pSticky)
	{
		pSticky->SetStickyGroup(this);
	}
	// vector�̌���
	_pSticky.reserve(_pSticky.size() + other->_pSticky.size());
	_pSticky.insert(_pSticky.end(), other->_pSticky.begin(), other->_pSticky.end());

	// ��������StickyGroup���X�V
	for(auto pMB : other->_pMeatBox)
	{
		pMB->SetStickyGroup(this);
	}
	// vector�̌���
	_pMeatBox.reserve(_pMeatBox.size() + other->_pMeatBox.size());
	_pMeatBox.insert(_pMeatBox.end(), other->_pMeatBox.begin(), other->_pMeatBox.end());

	// ����StickyGroup���폜
	_mode->RemoveGameObject(other);
}

void StickyGroup::AddSticky(Sticky* pSticky)
{
	if (pSticky == nullptr) return;

	_pSticky.push_back(pSticky);
	pSticky->SetStickyGroup(this);
}

bool StickyGroup::AddMeatBox(MeatBox* pMeatBox)
{
	if (pMeatBox == nullptr) return false;

	// ���łɒǉ�����Ă���ꍇ�͒ǉ����Ȃ�
	for (auto pMB : _pMeatBox)
	{
		if (pMB == pMeatBox) return false;
	}

	StickyGroup* tmpSG = pMeatBox->GetStickyGroup();
	if (tmpSG != nullptr) {
		// �~�[�g�{�b�N�X���ʂ�StickyGroup�ɏ������Ă���ꍇ�A��������
		if (tmpSG != this) {
			Merge(tmpSG);
			return true;
		}
	}
	// �~�[�g�{�b�N�X���܂�StickyGroup�ɏ������Ă��Ȃ��ꍇ
	else {
		_pMeatBox.push_back(pMeatBox);
		pMeatBox->SetStickyGroup(this);
		return true;
	}

	return false;
}
